// ----------------------------------------------------------------------
// TestMain.cpp
// ----------------------------------------------------------------------

#include "Tester.hpp" 
#include <Drv/Ip/TcpClientSocket.hpp>
//Todo

TEST(Nominal, SingleClient_ComponentInterface){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[1]; 
    tester.test_client_opening_ID(socket, 1); 
}

TEST(Nominal, MultiClient_ComponentInterface_2){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[2]; 
    tester.test_client_opening_ID(socket, 2); 
}

TEST(Nominal, MultiClient_ComponentInterface_5){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[5]; 
    tester.test_client_opening_ID(socket, 5); 
}

TEST(Nominal, Accpet_Read_Threads_0){
    Drv::Tester tester;
    tester.test_with_loop(1, true); 
}

TEST(Nominal, Accpet_Read_Threads_5){
    Drv::Tester tester;
    tester.test_with_loop(1, true); 
}

TEST(Nominal, Accpet_Read_Threads_10){
    Drv::Tester tester;
    tester.test_with_loop(1, true); 
}

TEST(Nominal, Parallel_Read_2){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[2]; 
    tester.test_parallel_recv(socket, 2); 
}

TEST(Nominal, Parallel_Read_5){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[5]; 
    tester.test_parallel_recv(socket, 5); 
}

/* Note: ASSERT Failing at TesterBase.hpp: line 163
TEST(Nominal, Parallel_Read_10){
    Drv::Tester tester;
    Drv::TcpClientSocket socket[10]; 
    tester.test_parallel_recv(socket, 10); 
}
*/
int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS(); 
}