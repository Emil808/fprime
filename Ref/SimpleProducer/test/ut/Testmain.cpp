#include "Tester.hpp"
TEST(Nominal, CMD_SET_NUM){
  Ref::Tester tester; 
  tester.testInvokeSET_NUM(); 
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}