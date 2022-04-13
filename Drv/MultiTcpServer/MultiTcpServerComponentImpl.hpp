// ======================================================================
// \title  MultiTcpServerComponentImpl.hpp
// \author Emil808
// \brief  hpp file for TcpServerComponentImpl component implementation class
//         Heavily inspired by Drv/TcpServer
// \copyright 
// ======================================================================

#ifndef MultiTcpServerComponentImpl_HPP
#define MultiTcpServerComponentImpl_HPP

#include <IpCfg.hpp>
#include <Drv/Ip/IpSocket.hpp>
#include <Drv/MultiTcpServer/MultiTcpServerSocket.hpp>
#include <Drv/MultiTcpServer/MTSTask.hpp>
#include <Drv/MultiTcpServer/MultiTcpServerManager.hpp>

#include "Drv/ByteStreamDriverModel/ByteStreamDriverModelComponentAc.hpp"

#include <Fw/Buffer/Buffer.hpp>
#include <Os/Task.hpp>

namespace Drv{

class MultiTcpServerComponentImpl : public ByteStreamDriverModelComponentBase, public AcceptClientTask {

    public: 
        // ----------------------------------------------------------------------
        // Construction, initialization, and destruction
        // ----------------------------------------------------------------------

        /** 
         * \brief construct the MultiTcpCLient component.
         * \param compName: name of this component
         */ 
        MultiTcpServerComponentImpl(const char* const compName); 

        /**
         * \brief Initialize this componnet
         * \param instance: instance number of this component
         */
        void init(const NATIVE_INT_TYPE instance = 0); 

        /**
         * \brief Deconstruct the component
         */
        ~MultiTcpServerComponentImpl(); 

        // ----------------------------------------------------------------------
        // Helper methods
        // ----------------------------------------------------------------------

        /**
         * \brief Configures the MultiTcpClient settings but does not open the connection
         *
         * The MultiTcpClientComponent needs to connect to a remote TCP server. This call configures the hostname, port and
         * send timeouts for that socket connection. This call should be performed on system startup before recv or send
         * are called. Note: hostname must be a dot-notation IP address of the form "x.x.x.x". DNS translation is left up
         * to the user.
         *
         * \param hostname: ip address of remote tcp server in the form x.x.x.x
         * \param port: port of remote tcp server
         * \param send_timeout_seconds: send timeout seconds component. Defaults to: SOCKET_TIMEOUT_SECONDS
         * \param send_timeout_microseconds: send timeout microseconds component. Must be less than 1000000. Defaults to:
         * SOCKET_TIMEOUT_MICROSECONDS
         * \return status of the configure
         */
        SocketIpStatus configure(const char* hostname,
                                const U16 port,
                                const U32 send_timeout_seconds = SOCKET_SEND_TIMEOUT_SECONDS,
                                const U32 send_timeout_microseconds = SOCKET_SEND_TIMEOUT_MICROSECONDS);

                            
        SocketIpStatus startup(); 

        void shutdown(); 

        MultiTcpServerManager& getSocketManager(); 

    PROTECTED:

        Fw::Buffer getBuffer(); 

        void sendBuffer(Fw::Buffer buffer, SocketIpStatus status); 

        void connected(); 


    
    PRIVATE:
        Drv::SendStatus send_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer); 

        Drv::PollStatus poll_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer); 

        Drv::MultiTcpServerManager m_manager; 
    

}; //class end
}; //namespace end
#endif //MultiTcpServerComponentImpl_HPP
