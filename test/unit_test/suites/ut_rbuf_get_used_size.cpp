//! \file ut_rbuf_get_used_size.cpp
//! \brief Ring rbuf get_used_size unit test
//! \date  2024-04
//! \author Nicolas Boutin

#include <gtest/gtest.h>

extern "C" {
#include "ring_buffer/ring_buffer.h"
}

using namespace testing;

class RBUF_GetUsedSize_Fixture : public ::testing::Test
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
 * \brief Get used size from empty rbuf
 */
TEST_F(RBUF_GetUsedSize_Fixture, get_used_size_001)
{
  EXPECT_EQ(RBUF_GetUsedSize(&rbuf), 0);
}

/**
 * \brief Get used size from rbuf with one byte
 */
TEST_F(RBUF_GetUsedSize_Fixture, get_used_size_002)
{
  RBUF_WriteUint8(&rbuf, 0xAA);

  EXPECT_EQ(RBUF_GetUsedSize(&rbuf), 1);
}

/**
 * \brief Get used size with rollover
 */
TEST_F(RBUF_GetUsedSize_Fixture, get_used_size_003)
{
  // Put two bytes in rbuf
  RBUF_WriteUint8(&rbuf, 0xAA);
  RBUF_WriteUint8(&rbuf, 0xAA);

  // Read two bytes
  RBUF_ReadUint8(&rbuf);
  RBUF_ReadUint8(&rbuf);

  // Put for 4 bytes
  RBUF_WriteUint8(&rbuf, 0xBB);
  RBUF_WriteUint8(&rbuf, 0xBB);
  RBUF_WriteUint8(&rbuf, 0xBB);
  RBUF_WriteUint8(&rbuf, 0xBB);

  EXPECT_EQ(RBUF_GetUsedSize(&rbuf), 4);
}
