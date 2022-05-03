#include <Drv/MultiTcpServer/SocketOpenTask.hpp>
#include <Fw/Logger/Logger.hpp>
#include <Fw/Types/Assert.hpp>
#include <cerrno>

#include <stdio.h> 
namespace Drv{

    SocketOpenTask::SocketOpenTask(): m_stop(false){}
    SocketOpenTask::~SocketOpenTask(){}

    void SocketOpenTask::startSocketOpenTask(const Fw::StringBase &name,
                    const bool reconnect ,
                    const NATIVE_INT_TYPE priority ,
                    const NATIVE_INT_TYPE stack ,
                    const NATIVE_INT_TYPE cpuAffinity ){
        
        FW_ASSERT(not m_task.isStarted()); 
        FW_ASSERT(not this->m_stop); 
    
        Os::Task::TaskStatus stat = m_task.start(name, SocketOpenTask::openTask, this, priority, stack, cpuAffinity);
        FW_ASSERT(Os::Task::TASK_OK == stat, static_cast<NATIVE_INT_TYPE>(stat)); 
    }

    void SocketOpenTask::stopSocketOpenTask(){
        this->m_stop = true; 
        this->getClientSocketManager().closeAll(); 
    }

    Os::Task::TaskStatus SocketOpenTask::joinSocketOpenTask(void** value_ptr){
        return m_task.join(value_ptr); 
    }

    void SocketOpenTask::openTask(void *pointer){
        FW_ASSERT(pointer); 
        SocketIpStatus status = SOCK_SUCCESS; 

        SocketOpenTask* self = reinterpret_cast<SocketOpenTask*>(pointer); 
        I32 index; 

        char line[25+1]; 
        char hostname_s[20] = {0}; 
        I32 port = -1; 
       

        //expected format of a line is 
            // "123.123.123.123:41244" <ip address>:port_Number [21+1]
       
        do{
            if(self->getHostFileEntry(line)){ //get a line
                //parse line into hostname and port
                index = sscanf(line, "%[^:]:%i", hostname_s, &port);
                if(index != 2){
                    continue; 
                }
                index = self->getClientSocketManager().getFreeSocketIndex(); 
                if(index >= 0){ //get free socket returned with valid index of an open socket

                    self->getClientSocketManager().getSocketHandler(index).setHost(hostname_s, static_cast<U16>(port)); 

                    status = self->getClientSocketManager().getSocketHandler(index).open(); //attempt opening the connection
                    if(status == SOCK_SUCCESS){
                        status = self->getClientSocketManager().getSocketHandler(index).sendClientID(self->getDeviceID()); //send clientID
                        if(status == SOCK_SUCCESS){
                            status = self->getClientSocketManager().getSocketHandler(index).recvSrvrID(); //recv serverID

                            //check if another socket is already connected to the Server
                            for(int i = 0; i < MAX_CLIENTS; i++){
                                if(index == i){ continue; }
                                if(self->getClientSocketManager().getSocketHandler(i).getServerDeviceID() == self->getClientSocketManager().getSocketHandler(index).getServerDeviceID()){
                                    status = SOCK_DISCONNECTED; 
                                    break; 
                                }
                            }

                        }
                    }

                    if(status != SOCK_SUCCESS){
                        self->getClientSocketManager().close(index); 
                    }  
                    else{
                        self->connected(); //invoke ready port
                    }  
                }
            }
            //if all sockets are open then get free socket returned -1
                //do nothing, attempt to get free socket again. 
        }while(not self->m_stop);
        self->getClientSocketManager().closeAll(); 
    }
}