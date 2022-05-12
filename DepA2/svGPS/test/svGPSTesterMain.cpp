
#include "svGPSTester.hpp"

TEST(Single, SingeRun){
    DepA2::Tester tester; 
    tester.runSingle(); 
}


int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS(); 
}