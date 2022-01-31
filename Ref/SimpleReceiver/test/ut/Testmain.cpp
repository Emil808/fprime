#include "Tester.hpp"
TEST(Nominal, SR_1_val){
  Ref::Tester tester; 
  tester.testSingle(); 
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}