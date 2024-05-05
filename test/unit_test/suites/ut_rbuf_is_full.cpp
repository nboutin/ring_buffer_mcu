//! \file ut_rbuf_is_full.cpp
//! \brief Ring rbuf is_full unit test
//! \date  2024-04
//! \author Nicolas Boutin

#include <gtest/gtest.h>

extern "C" {
#include "ring_buffer/ring_buffer.h"
}

using namespace testing;

class RBUF_IsFull_Fixture : public ::testing::Test
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
 * \brief not full after initialization
 */
TEST_F(RBUF_IsFull_Fixture, is_full_001)
{
  EXPECT_EQ(RBUF_IsFull(&rbuf), false);
}

/**
 * \brief full after adding elements
 */
TEST_F(RBUF_IsFull_Fixture, is_full_002)
{
  for (int i = 0; i < DATA_SIZE; ++i)
  {
    RBUF_WriteUint8(&rbuf, 0xAA);
  }

  EXPECT_EQ(RBUF_IsFull(&rbuf), true);
}
