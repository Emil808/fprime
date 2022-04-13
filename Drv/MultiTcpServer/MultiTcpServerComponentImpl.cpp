#include <Drv/MultiTcpServer/MultiTcpServerComponentImpl.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include "Fw/Types/Assert.hpp"
namespace Drv{
    
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    MultiTcpServerComponentImpl::MultiTcpServerComponentImpl(const char* const compName)
        : ByteStreamDriverModelComponentBase(compName), AcceptClientTask()
    {}

    void MultiTcpServerComponentImpl::init(const NATIVE_INT_TYPE instance){
        ByteStreamDriverModelComponentBase::init(instance); 
    }

    MultiTcpServerComponentImpl::~MultiTcpServerComponentImpl(){}

    SocketIpStatus MultiTcpServerComponentImpl::configure(const char* hostname,
                                const U16 port,
                                const U32 send_timeout_seconds ,
                                const U32 send_timeout_microseconds ){
        return this->m_manager.configure(hostname, port, send_timeout_seconds, send_timeout_microseconds); 
    }

    SocketIpStatus MultiTcpServerComponentImpl::startup(){
        return this->m_manager.startup(); 
    }

    void MultiTcpServerComponentImpl::shutdown(){
        this->m_manager.shutdown(); 
    }

    Fw::Buffer MultiTcpServerComponentImpl::getBuffer(){
        return allocate_out(0, 1024); 
    }

    void MultiTcpServerComponentImpl::sendBuffer(Fw::Buffer buffer, SocketIpStatus status){
        Drv::RecvStatus recvStatus = (status == SOCK_SUCCESS) ? RecvStatus::RECV_OK : RecvStatus::RECV_ERROR;
        this->recv_out(0, buffer, recvStatus);
    }

     Drv::SendStatus MultiTcpServerComponentImpl::send_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer){
         FW_ASSERT(0); 
         return SendStatus::SEND_ERROR; 
     }

    Drv::PollStatus MultiTcpServerComponentImpl::poll_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer){
        FW_ASSERT(0); 
        return PollStatus::POLL_ERROR; 
    }

    MultiTcpServerManager& MultiTcpServerComponentImpl::getSocketManager(){
        return m_manager; 
    }

    void MultiTcpServerComponentImpl::connected(){
        if (isConnected_ready_OutputPort(0)) {
        this->ready_out(0);
    }
    }
}