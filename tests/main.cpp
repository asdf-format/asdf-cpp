#include "gtest/gtest.h"

std::string test_data_path;

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  /* Configure test data path for all unit tests */
  assert (argc == 2);
  test_data_path = std::string(argv[1]) + "/";

  return RUN_ALL_TESTS();
}
