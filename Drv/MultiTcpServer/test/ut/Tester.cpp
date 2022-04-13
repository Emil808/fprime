// ======================================================================
// \title  ByteStreamDriverModel.hpp
// \author emil
// \brief  cpp file for ByteStreamDriverModel test harness implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#include "Tester.hpp"
#include "STest/Pick/Pick.hpp"
#include "Os/Log.hpp"
#include <Drv/Ip/test/ut/PortSelector.hpp>
#include <Drv/Ip/test/ut/SocketTestHelper.hpp>
#include <Os/Task.hpp>

#include <stdio.h>

#define INSTANCE 0
#define MAX_HISTORY_SIZE 10

namespace Drv {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  Tester ::
    Tester() :
      ByteStreamDriverModelGTestBase("Tester", MAX_HISTORY_SIZE),
      component("ByteStreamDriverModel"), m_data_buffer(m_data_storage, 0)
  {
    this->initComponents();
    this->connectPorts();
    ::memset(m_data_storage, 0, sizeof(m_data_storage));
  }

  Tester ::
    ~Tester()
  {

  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------
  void Tester::test_component_interface(U32 n_clients){
    /*
    U8 buffer[sizeof(m_data_storage)] = {};
    Drv::SocketIpStatus status1 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus status2 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus serverStat = Drv::SOCK_SUCCESS;
    I32 size = sizeof(m_data_storage);
    NATIVE_INT_TYPE fd; 

    //configure server
    U16 port = Drv::Test::get_free_port(); 
    ASSERT_NE(0, port); 

    this->component.configure("127.0.0.1", port, 0, 100); 
    serverStat = this->component.startup();               // Listening for clients starts here
    EXPECT_EQ(serverStat, SOCK_SUCCESS);

    ASSERT_EQ(this->component.getNConnected(), 0);        //No clients connected yet

    // set up client
    for(U32 i = 0; i < n_clients; i++){
    Drv::TcpClientSocket client; 
    client.configure("127.0.0.1", port, 0, 100); 
    status2  = client.open(); 
    // get client to connect
    ASSERT_EQ(status2, SOCK_SUCCESS);

    status1 = this->component.getSocketHandler().open(fd); 

    ASSERT_EQ(status1, SOCK_SUCCESS); 
    
    //client send something to receiver
    Drv::Test::force_recv_timeout(this->component.getSocketHandler());
    Drv::Test::force_recv_timeout(client);
    this->m_data_buffer.setSize(sizeof(this->m_data_storage));
    Drv::Test::fill_random_buffer(this->m_data_buffer);
    status2 = client.send(this->m_data_buffer.getData(), this->m_data_buffer.getSize()); 

    ASSERT_EQ(status2, SOCK_SUCCESS); 

    status1 = this->component.recv(fd, buffer, size); //not receiving properly? 
    ASSERT_EQ(status1, Drv::SOCK_SUCCESS);
    ASSERT_EQ(size, m_data_buffer.getSize());
    Drv::Test::validate_random_buffer(m_data_buffer, buffer);
    client.close(); 

    }

    this->component.shutdown(); 
    */


  }
  void Tester::test_socket_manager(){

  

  }

  void Tester::test_client_opening_ID(Drv::TcpClientSocket* client, U32 n_clients){
    // U8 buffer[sizeof(m_data_storage)] = {};
    Drv::SocketIpStatus status1 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus status2 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus serverStat = Drv::SOCK_SUCCESS;
    // I32 size = sizeof(m_data_storage);
    // NATIVE_INT_TYPE fd; 

    U8 clientID[6] = "ID"; 

    //configure server
    U16 port = Drv::Test::get_free_port(); 
    ASSERT_NE(0, port); 

    this->component.configure("127.0.0.1", port, 0, 100); 
    serverStat = this->component.startup();               // Listening for clients starts here
    EXPECT_EQ(serverStat, SOCK_SUCCESS);

    I32 client_handler_index; 
    for(U32 i = 0; i < n_clients; i++){
      client[i].configure("127.0.0.1", port, 0, 100); 
        
      status2  = client[i].open();
      ASSERT_EQ(status2, SOCK_SUCCESS); 

      status1 = this->component.getSocketManager().open(client_handler_index); 

      ASSERT_EQ(status1, SOCK_SUCCESS); 
      
      //set up and RNG a device ID
      
      generate_device_id(clientID); 
      status2 = client[i].send(clientID, sizeof(clientID)); //client sends ID

      status1 = this->component.getSocketManager().getSocketHandler(client_handler_index).recvClientID();          //server receives and saves ID

      ASSERT_EQ(status2, SOCK_SUCCESS); 
      ASSERT_EQ(status1, SOCK_SUCCESS); 

      U32 receivedID = this->component.getSocketManager().getSocketHandler(client_handler_index).getClientDeviceID(); 

      verify_device_id(clientID, receivedID); 

      ASSERT_EQ(this->component.getSocketManager().getNConnected(), 1); 
      client[i].close(); 
      this->component.getSocketManager().close(client_handler_index); 
      ASSERT_EQ(this->component.getSocketManager().getNConnected(), 0);
    }
      this->component.getSocketManager().closeAll(); 
      this->component.shutdown(); 

  }
  void Tester::test_with_loop(U32 iterations, bool recv_thread)
  {
    //U8 buffer[sizeof(m_data_storage)] = {};
    //Drv::SocketIpStatus status1 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus status2 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus serverStat = Drv::SOCK_SUCCESS;

    U16 port =  Drv::Test::get_free_port();
    ASSERT_NE(0, port);

     this->component.configure("127.0.0.1", port, 0, 100);
     serverStat = this->component.startup();                // Server starts listening here at success
     EXPECT_EQ(serverStat, SOCK_SUCCESS);

    // start accept task
    if (recv_thread){
      Os::TaskString name("Accept Client Thread");
      this->component.startAcceptTask(name, true, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT); 
    }

    for (U32 i = 0; i < iterations && serverStat == SOCK_SUCCESS; i++){
      Drv::TcpClientSocket client; 
      client.configure("127.0.0.1", port, 0, 100);
      status2 = client.open(); 
      //I32 size = sizeof(m_data_storage); 
      ASSERT_EQ(status2, SOCK_SUCCESS); 

      Os::Task::delay(10); //Delay to let other threads get to update Nconnected
      EXPECT_EQ(this->component.getSocketManager().getNConnected(), 1); 
      EXPECT_EQ(this->component.getTaskCount(), 1); 

      m_data_buffer.setSize(sizeof(m_data_storage));
      Drv::Test::fill_random_buffer(m_data_buffer);

      m_spinner = false;
      m_data_buffer.setSize(sizeof(m_data_storage));
      client.send(m_data_buffer.getData(), m_data_buffer.getSize());
      while (not m_spinner) {}

      client.close(); 

      // Want to do detect closure, but currently failing? 
      // Expecting getNConnected() to be 0 after close. 
    }
    this->component.shutdown();
    EXPECT_EQ(this->component.getSocketManager().getNConnected(), 0);
    this->component.stopAcceptTask(); 
    this->component.joinAcceptTask(nullptr); 
    
  }

  void Tester::test_parallel_recv(Drv::TcpClientSocket* socket_array, U32 n_clients){
    //Drv::SocketIpStatus status2 = Drv::SOCK_SUCCESS;
    Drv::SocketIpStatus serverStat = Drv::SOCK_SUCCESS;
    I32 client_count = static_cast<I32>(n_clients); 
    U16 port =  Drv::Test::get_free_port();
    ASSERT_NE(0, port);

     this->component.configure("127.0.0.1", port, 0, 100);
     serverStat = this->component.startup();                // Server starts listening here at success
     EXPECT_EQ(serverStat, SOCK_SUCCESS);

    Os::TaskString name("Accept Client Thread");
    this->component.startAcceptTask(name, true, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT); 
    
    //open multiple clients
    //have clients send, which invokes the recv thread

    for(int i = 0; i < client_count; i++){
      socket_array[i].configure("127.0.0.1", port, 0, 100);
      ASSERT_EQ(socket_array[i].open(), SOCK_SUCCESS); 
      Os::Task::delay(10); //delay for other threads to update N-connected
      EXPECT_EQ(this->component.getSocketManager().getNConnected(), i+1); 
    }
     
    
    ASSERT_EQ(this->component.getSocketManager().getNConnected(), n_clients); 

    for(int i = 0; i < client_count; i++){
      m_spinner = false;

      m_data_buffer.setSize(sizeof(m_data_storage));
      Drv::Test::fill_random_buffer(m_data_buffer);
      socket_array[i].send(m_data_buffer.getData(), m_data_buffer.getSize()); 
      while (not m_spinner) {}
    }


    for(int i = 0; i < client_count; i++){
      socket_array[i].close(); 
    }

    this->component.shutdown();
    EXPECT_EQ(this->component.getSocketManager().getNConnected(), 0);
    this->component.stopAcceptTask(); 
    this->component.joinAcceptTask(nullptr); 
  }

  void Tester::generate_device_id(U8* idBuffer){
    int size = sizeof(idBuffer); 
    if(size == 6){
      idBuffer[2] = STest::Pick::lowerUpper(0, 0xFF); 
      idBuffer[3] = STest::Pick::lowerUpper(0, 0xFF); 
      idBuffer[4] = STest::Pick::lowerUpper(0, 0xFF); 
      idBuffer[5] = STest::Pick::lowerUpper(0, 0xFF); 
    }
  } 

  void Tester::verify_device_id(U8* clientBuffer, U32 serverRecv){
    int size = sizeof(clientBuffer); 
    if(size == 6){
      ASSERT_EQ(clientBuffer[5], (serverRecv & 0xFF)>>0); 
      ASSERT_EQ(clientBuffer[4], (serverRecv & 0xFF00)>>8);
      ASSERT_EQ(clientBuffer[3], (serverRecv & 0xFF0000)>>16);
      ASSERT_EQ(clientBuffer[2], (serverRecv & 0xFF000000)>>24);
    }
  }
  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Fw::Buffer Tester ::
    from_allocate_handler(
        const NATIVE_INT_TYPE portNum,
        U32 size
    )
  {
    this->pushFromPortEntry_allocate(size);
    // TODO: Return a value
    Fw::Buffer buffer(new U8[size], size); 
    m_data_buffer2 = buffer; 
    return buffer; 
  }

  void Tester ::
    from_deallocate_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    this->pushFromPortEntry_deallocate(fwBuffer);
  }

  void Tester ::
    from_ready_handler(
        const NATIVE_INT_TYPE portNum
    )
  {
    this->pushFromPortEntry_ready();
  }

  void Tester ::
    from_recv_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &recvBuffer,
        const Drv::RecvStatus &recvStatus
    )
  {
    this->pushFromPortEntry_recv(recvBuffer, recvStatus);

    EXPECT_EQ(m_data_buffer.getSize(), recvBuffer.getSize()) << "Invalid transmission size";
    Drv::Test::validate_random_buffer(m_data_buffer, recvBuffer.getData());
    m_spinner = true;
    delete[] recvBuffer.getData();
  }

  // ----------------------------------------------------------------------
  // Helper methods
  // ----------------------------------------------------------------------

  void Tester ::
    connectPorts()
  {

    // poll
    this->connect_to_poll(
        0,
        this->component.get_poll_InputPort(0)
    );

    // send
    this->connect_to_send(
        0,
        this->component.get_send_InputPort(0)
    );

    // allocate
    this->component.set_allocate_OutputPort(
        0,
        this->get_from_allocate(0)
    );

    // deallocate
    this->component.set_deallocate_OutputPort(
        0,
        this->get_from_deallocate(0)
    );

    // ready
    this->component.set_ready_OutputPort(
        0,
        this->get_from_ready(0)
    );

    // recv
    this->component.set_recv_OutputPort(
        0,
        this->get_from_recv(0)
    );




  }

  void Tester ::
    initComponents()
  {
    this->init();
    this->component.init(
        INSTANCE
    );
  }

} // end namespace Drv
