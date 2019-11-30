// GoogleTest includes
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#include <gtest/gtest.h>


//=============================================================================
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
