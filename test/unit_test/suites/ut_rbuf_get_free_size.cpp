//! \file ut_rbuf_get_free_size.cpp
//! \brief Ring rbuf get_free_size unit test
//! \date  2024-04
//! \author Nicolas Boutin

#include <gtest/gtest.h>

extern "C" {
#include "ring_buffer/ring_buffer.h"
}

using namespace testing;

class RBUF_GetFreeSize_Fixture : public ::testing::Test
{
protected:
  void SetUp()
  {
    RBUF_InitEmpty(&rbuf, data, BUFFER_SIZE);
  }
  // attributes
  RBUF_t rbuf;
  static constexpr uint8_t DATA_SIZE   = 5;
  static constexpr uint8_t BUFFER_SIZE = DATA_SIZE + 1;
  std::uint8_t data[BUFFER_SIZE];
};

/**
 * \brief Get free size from empty rbuf
 */
TEST_F(RBUF_GetFreeSize_Fixture, get_free_size_001)
{
  EXPECT_EQ(RBUF_GetFreeSize(&rbuf), DATA_SIZE);
}

/**
 * \brief Get free size from rbuf with one byte used
 */
TEST_F(RBUF_GetFreeSize_Fixture, get_free_size_002)
{
  RBUF_WriteUint8(&rbuf, 0xAA);
  EXPECT_EQ(RBUF_GetFreeSize(&rbuf), DATA_SIZE - 1);
}

/**
 * \brief Get free size from full rbuf
 */
TEST_F(RBUF_GetFreeSize_Fixture, get_free_size_003)
{
  for (uint8_t i = 0; i < DATA_SIZE; i++)
  {
    RBUF_WriteUint8(&rbuf, 0xAA);
  }
  EXPECT_EQ(RBUF_GetFreeSize(&rbuf), 0);
}
