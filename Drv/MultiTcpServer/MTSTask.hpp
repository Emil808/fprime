#ifndef DRV_MULTITHREADEDTCPSERVERTASK_HPP
#define DRV_MULTITHREADEDTCPSERVERTASK_HPP

#include <Fw/Buffer/Buffer.hpp>
#include <Os/Task.hpp>
#include <Os/Mutex.hpp>
#include <Drv/Ip/IpSocket.hpp>
#include <Drv/MultiTcpServer/MultiTcpServerManager.hpp>

#define MAX_TASKS 16

namespace Drv{
class AcceptClientTask{

    public: 
        AcceptClientTask(); 
        virtual ~AcceptClientTask(); 

        void startAcceptTask(const Fw::StringBase &name,
                             const bool reconnect = true,
                             const NATIVE_INT_TYPE priority = -1,
                             const NATIVE_INT_TYPE stack = -1,
                             const NATIVE_INT_TYPE cpuAffinity = -1);

        void stopAcceptTask(); 
        Os::Task::TaskStatus joinAcceptTask(void** value_ptr); 

        static void acceptTask(void* pointer); 
        static void readTask(void* pointer); 

        // Helper functions for testing
        U32 getTaskCount(); 
    PROTECTED:

    
        virtual MultiTcpServerManager& getSocketManager() = 0; //todo in compimpl
        
        void startReadTask(I32 task_index); 
        SocketIpStatus open(I32 index); //
        void close(I32 index);          //
        void stopReadTask(I32 index);   //
        Os::Task::TaskStatus joinReadTask(I32 index, void** value_ptr); 
        Drv::MultiTcpServerSocket& getSocketHandler(I32 index);

        virtual Fw::Buffer getBuffer() = 0; //
        
        virtual void sendBuffer(Fw::Buffer buffer, SocketIpStatus status) = 0; //
        virtual void connected() = 0; //
    private:
        Os::Task main_task;
        Os::Task task_pool[MAX_TASKS];  
        I32 taskCount; 
        bool m_stop;
        bool m_reconnect; 

        NATIVE_INT_TYPE task_priority; 
        NATIVE_INT_TYPE task_stack;
        NATIVE_INT_TYPE task_affinity; 
        Os::Mutex m_lock; 

};

class MultiReadTask{
    public:
        MultiReadTask(); 
        ~MultiReadTask(); 
        void setIndex(I32 index); 
        bool isStarted(); 

        void startSocketTask(const Fw::StringBase &name,
                         const bool reconnect = true,
                         const NATIVE_INT_TYPE priority = -1,
                         const NATIVE_INT_TYPE stack = -1,
                         const NATIVE_INT_TYPE cpuAffinity = -1);


        SocketIpStatus open(); 
        void close(); 
        void stopSocketTask(); 

        Os::Task::TaskStatus joinSocketTask(void** value_ptr); 

    PROTECTED:

        virtual Drv::MultiTcpServerSocket& getSocketHandler(I32 index) = 0; 
        virtual Fw::Buffer getBuffer() = 0; 
        virtual void sendBuffer(Fw::Buffer buffer, SocketIpStatus status) = 0; 
        virtual void connected() = 0; 

        static void readTask(void* pointer); 
    private:
    I32 index; 
    bool m_stop; 
    Os::Task m_task; 



};
}
#endif