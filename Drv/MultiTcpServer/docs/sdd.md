\page DrvMultiTcpServer Multi Threaded Tcp Server Component
# Drv::MultiTcpServer Multi Threaded Tcp Server Component

The TCP server component bridges the byte stream driver model interface to multiple remote TCP client to which this tcp server
connects and sends/receives bytes. It implements the callback formation (shown below). 

Since it is a server, it must startup and listen for client connections. 

To accept multiple clients and start their threads, one thread, the Accept Thread, will hold on open, which will callback to 
accepting a client. Here the thread will be blocked on accept, until a client connects. Once a client connects and moves past
accept, the accept task will find an available Task handler and start the read task for that client.


For more information on the supporting TCP implementation see: Drv::TcpServerSocket.
For more information on the ByteStreamModelDriver see: Drv::ByteStreamDriverModel.

## Design

The manager component (typically the ground interface) initiates the transfer of send data by calling the "send" port.
The caller will provide a `Fw::Buffer` containing the data to send and the port call will return a status of that send.
These responses are an enumeration whose values are described in the following table:

| Value | Description |
|---|---|
| Drv::SEND_OK    | Send functioned normally. |
| Drv::SEND_RETRY | Send should be retried, but a subsequent send should return SEND_OK. |
| Drv::SEND_ERROR | Send produced an error, future sends likely to fail. |

This data is immediately sent out to the remote tcp server with a configured send timeout. See Usage described below.

**Callback Formation**

![Callback](../../ByteStreamDriverModel/docs/img/canvas-callback.png)

In the callback formation, the byte stream driver component initiates the transfer of received data by calling the
"readCallback" output port. This port transfers any read data in a `Fw::Buffer` along with a status for the receive.
This status is an enumeration whose values are described in the following table:

| Value | Description |
|---|---|
| Drv::RECV_OK    | Receive functioned normally buffer contains valid data. |
| Drv::RECV_ERROR | Receive produced an error and buffer contains no valid data. |

## Usage

The Drv::MultiTcpServerComponentImpl must be configured with the address of the remote connection, and the socket must be
open to begin. Usually, the user runs the Drv::MultiTcpServerComponentImpl engaging its Accept thread, which will listen for connections, accepts, and delegates reading from that specific client onto a
read thread. The component is passive and has no commands meaning users should `init`, `configure`, and
`startAcceptTask`. In addition to these methods shared with the Drv::MultiTcpClientComponentImpl, the server provides
`startup` and `shutdown` methods to start and stop the listening socket. It `startup` must be run before the Accept task
is started and `shutdown` should be called before the task is stopped.

Upon shutdown, the `stopAcceptThread` and `joinAcceptThread` methods should be called to ensure
proper resource deallocation. This typical usage is shown in the C++ snippet below.


```c++
Drv::MultiTcpServerComponentImpl comm = Drv::MultiTcpServerComponentImpl("TCP Server");

bool constructApp(bool dump, U32 port_number, char* hostname) {
    ...
    comm.init(0);
    ...
    if (hostname != nullptr && port_number != 0) {
        Os::TaskString name("AcceptTask");
        comm.configure(hostname, port_number);
        comm.startup();
        comm.startAcceptTask(name);
    }
}

void exitTasks() {
    ...
    comm.shutdown();
    comm.stopAccepTask();
    (void) comm.joinAcceptTask(nullptr);
}
```
## Class Diagram
![class diagram](./img/class_diagram_tcpserver.png)

## Requirements

| Name | Description | Validation |
|---|---|---|
| TCP-SERVER-COMP-001 | The tcp server component shall implement the ByteStreamDriverModel  | inspection |
| TCP-SERVER-COMP-002 | The tcp server component shall provide a read thread | unit test |
| TCP-SERVER-COMP-003 | The tcp server component shall provide bidirectional communication with a tcp client | unit test |

## Change Log

| Date | Description |
|---|---|
| 2020-12-21 | Initial Draft |
| 2021-01-28 | Updated |
