//! \file ut_rbuf_is_empty.cpp
//! \brief Ring rbuf is_empty unit test
//! \date  2024-04
//! \author Nicolas Boutin

#include <gtest/gtest.h>

extern "C" {
#include "ring_buffer/ring_buffer.h"
}

using namespace testing;

class RBUF_IsEmpty_Fixture : public ::testing::Test
{
protected:
  void SetUp()
  {
    RBUF_InitEmpty(&rbuf, data, DATA_SIZE);
  }
  // attributes
  RBUF_t rbuf;
  static constexpr uint8_t DATA_SIZE = 5;
  std::uint8_t data[DATA_SIZE];
};

/**
 * \brief empty after initialization
 */
TEST_F(RBUF_IsEmpty_Fixture, is_empty_001)
{
  EXPECT_EQ(RBUF_IsEmpty(&rbuf), true);
}

/**
 * \brief not empty after adding one byte to rbuf
 */
TEST_F(RBUF_IsEmpty_Fixture, is_empty_002)
{
  RBUF_WriteUint8(&rbuf, 0xAA);
  EXPECT_EQ(RBUF_IsEmpty(&rbuf), false);
}
