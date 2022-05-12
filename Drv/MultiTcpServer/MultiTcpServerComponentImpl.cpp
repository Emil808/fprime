#include <Drv/MultiTcpServer/MultiTcpServerComponentImpl.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include "Fw/Types/Assert.hpp"
#include <Fw/Types/StringUtils.hpp>
#include <Fw/Logger/Logger.hpp>
#include <stdio.h>
#include <inttypes.h>
#include <cerrno> 
namespace Drv{
    
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    MultiTcpServerComponentImpl::MultiTcpServerComponentImpl(const char* const compName)
        : ByteStreamDriverModelComponentBase(compName), AcceptClientTask(), SocketOpenTask(), fp(NULL)
    {}

    void MultiTcpServerComponentImpl::init(const NATIVE_INT_TYPE instance){
        ByteStreamDriverModelComponentBase::init(instance); 
    }

    MultiTcpServerComponentImpl::~MultiTcpServerComponentImpl(){}

    SocketIpStatus MultiTcpServerComponentImpl::configure(const char* hostname,
                                const U16 port,
                                const U32 send_timeout_seconds ,
                                const U32 send_timeout_microseconds ){
        (void) this->m_CManager.configure(send_timeout_seconds, send_timeout_microseconds); 
        return this->m_manager.configure(hostname, port, send_timeout_seconds, send_timeout_microseconds); 
    }

    SocketIpStatus MultiTcpServerComponentImpl::startup(){
        return this->m_manager.startup(); 
    }

    void MultiTcpServerComponentImpl::shutdown(){
        fclose(fp); 
        this->m_manager.shutdown(); 
        this->m_CManager.closeAll(); 
    }

    Fw::Buffer MultiTcpServerComponentImpl::getBuffer(){

        //todo: handle failed allocate_out from Buffer Manager
        //should it be at this level? or at the Task level? 
        return allocate_out(0, 1024); 
    }

    void MultiTcpServerComponentImpl::sendBuffer(Fw::Buffer buffer, SocketIpStatus status){
        Drv::RecvStatus recvStatus = (status == SOCK_SUCCESS) ? RecvStatus::RECV_OK : RecvStatus::RECV_ERROR;
        this->recv_out(0, buffer, recvStatus);
    }

     Drv::SendStatus MultiTcpServerComponentImpl::send_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer){
        // ID Protocol:
        // "IDXXXX" - Sender's ID
        // "IDXXXX" - Receiver's ID
        // {} - Data from component.
        // if using ID protocol

        U32 index = 0; 
        U32 receiverID = 0; 
        //char ID_string[5] = {0};
        if(this->isUsingIdProtocol()){
            U8* data = fwBuffer.getData(); 
            U32 size = fwBuffer.getSize(); 
        
            for(U32 i = 0; i < size; i++){              // go through buffer
                if(data[i] == 'I' && data[i+1] == 'D'){ // find ID tag
                    if (receiverID == 0){            // if first ID tag, it is senders ID, mark that we found first ID tag
                        receiverID = 1; 
                    }
                    else{               // found second ID tag
                        //ID_string[0] = data[i+2]; ID_string[1] = data[i+3]; ID_string[2] = data[i+4]; ID_string[3] = data[i+5]; 
                        //receiverID = strtoumax(ID_string, NULL, 10); 
                        receiverID = (data[i+2] << 24) + (data[i+3] << 16) + (data[i+4] << 8) + (data[i+5] << 0); 
                        index = i; 
                        break; 
                    }
                }
            }
            if(data[index] != 'I' && data[index+1] != 'D'){ // Id Protocol header incorrect
                deallocate_out(0, fwBuffer); 
                return SendStatus::SEND_ERROR; 
            }        
        }
        Drv::SocketIpStatus status;
        if(receiverID != 808464432){    // receiverID is not 808464432, send to a specific connection
            status = m_CManager.getSocketHandlerByID(receiverID).send(fwBuffer.getData(), fwBuffer.getSize()); 
            deallocate_out(0, fwBuffer); 

            if ((status == SOCK_DISCONNECTED) || (status == SOCK_INTERRUPTED_TRY_AGAIN)) {
            return SendStatus::SEND_RETRY;
            } else if (status != SOCK_SUCCESS) {
                return SendStatus::SEND_ERROR;
            }
            return SendStatus::SEND_OK;
        }
        else{ // receiverID is 808464432, send Broadcast

            for(int i = 0; i < MAX_CLIENTS; i++){
                if(m_CManager.getSocketHandler(i).isOpened()){
                    status = m_CManager.getSocketHandler(i).send(fwBuffer.getData(), fwBuffer.getSize()); 
                }
            }
            deallocate_out(0, fwBuffer); 
            if (status != SOCK_SUCCESS){
                return SendStatus::SEND_ERROR; 
            }
            return SendStatus::SEND_OK; 
        }
        
    }

    Drv::PollStatus MultiTcpServerComponentImpl::poll_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer){
        FW_ASSERT(0); 
        return PollStatus::POLL_ERROR; 
    }

    MultiTcpServerManager& MultiTcpServerComponentImpl::getSocketManager(){
        return m_manager; 
    }

    TcpClientManager& MultiTcpServerComponentImpl::getClientSocketManager(){
        return m_CManager; 
    }

    void MultiTcpServerComponentImpl::setDeviceID(U32 ID){
        FW_ASSERT(ID); 
        FW_ASSERT(ID != 0x30303030); //ID is not broadcast ID

        DeviceID = ID; 
    }
    U32 MultiTcpServerComponentImpl::getDeviceID(){
        return DeviceID; 
    }
    void MultiTcpServerComponentImpl::connected(){
        if (isConnected_ready_OutputPort(0)) {
        this->ready_out(0);
        }
    }

    bool MultiTcpServerComponentImpl::getState(){
        bool current_state; 
        this->state_lock.lock(); 
            current_state = this->state;
        this->state_lock.unLock(); 
        return current_state; 
    }

    void MultiTcpServerComponentImpl::setHostFile(const char* filename, U32 size){
        FW_ASSERT(filename); 
        FW_ASSERT(size < MAX_HOSTFILE_NAME_SIZE); 
        
        (void) Fw::StringUtils::string_copy(this->hostfile_name, filename, MAX_HOSTFILE_NAME_SIZE); 
        
        this->fp = fopen(this->hostfile_name, "r"); 
        if(this->fp == NULL){
            (void)fprintf(stderr,"fopen on %s returned NULL ERRNO: %d\n", this->hostfile_name, errno);
            FW_ASSERT(this->fp); 
        }
    }

    bool MultiTcpServerComponentImpl::getHostFileEntry(char* entry){
        FW_ASSERT(this->hostfile_name); 
        FW_ASSERT(entry); 
        bool valid = false; 

        char line[25+1] = "";  
       
        if(fscanf(fp, "%25s", line) > 0){
            valid = true; 
        }

        if(line[0] == 0x00){ //reached EOF
            valid = false; 
            rewind(fp); 
            
        
            if(fscanf(fp, "%25s", line) > 0){
                valid = true; 
            }
        }
        (void) Fw::StringUtils::string_copy(entry, line, 25+1); 

        //fprintf(stderr, "Processing line %s\n", entry); 
          // fclose(fp); 
        
        return valid; 
    }
}