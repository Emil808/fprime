#ifndef DRV_MTSMANAGER_HPP
#define DRV_MTSMANAGER_HPP

#include <Fw/Types/BasicTypes.hpp>
#include <Drv/MultiTcpServer/MultiTcpServerSocket.hpp> // for SocketIpStatus Enumeration
#include <IpCfg.hpp> 
#include <Os/Mutex.hpp>

namespace Drv{
    class MultiTcpServerManager{
        public:
            MultiTcpServerManager();
            ~MultiTcpServerManager(); 
            
            SocketIpStatus configure(const char* hostname, const U16 port, const U32 send_timeout_seconds,
                             const U32 send_timeout_microseconds);
            SocketIpStatus startup();
            SocketIpStatus open(NATIVE_INT_TYPE& index); 
            void shutdown(); 
            void closeServer(); 
            void closeAll();
            void close(NATIVE_INT_TYPE index); 

            NATIVE_INT_TYPE getServerFd(); 
            I32 getNConnected(); 

            Drv::MultiTcpServerSocket& getSocketHandler(U32 socket_index); 

        private:
            
            NATIVE_INT_TYPE m_base_fd; 
            U32 m_timeoutSeconds; 
            U32 m_timeoutMicroseconds; 
            U16 m_port; 
            bool m_open; 
            char m_hostname[SOCKET_MAX_HOSTNAME_SIZE]; 
            NATIVE_INT_TYPE current_client_connections; 
            Drv::MultiTcpServerSocket sockets[MAX_CLIENTS]; 

            //Todo set up Os::Mutex

    };
}
#endif