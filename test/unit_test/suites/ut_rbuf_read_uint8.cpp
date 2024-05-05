//! \file ut_rbuf_read_uint8.cpp
//! \brief Ring rbuf unit test
//! \date  2023-04
//! \author Nicolas Boutin

#include <gtest/gtest.h>

extern "C" {
#include "ring_buffer/ring_buffer.h"
}

using namespace testing;

class RBUF_ReadUint8_Fixture : public ::testing::Test
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
 * \brief Read empty buffer
 */
TEST_F(RBUF_ReadUint8_Fixture, read_uint8_001)
{
  auto u8 = RBUF_ReadUint8(&rbuf);
  EXPECT_EQ(u8, 0);
  EXPECT_EQ(rbuf.read_index, 0);
}

/**
 * \brief Read one byte
 */
TEST_F(RBUF_ReadUint8_Fixture, read_uint8_002)
{
  RBUF_WriteUint8(&rbuf, 0xAA);
  auto u8 = RBUF_ReadUint8(&rbuf);
  EXPECT_EQ(u8, 0xAA);
  EXPECT_EQ(rbuf.read_index, 1);
}

/**
 * \brief Read with rollover
 */
TEST_F(RBUF_ReadUint8_Fixture, read_uint8_003)
{
  RBUF_WriteUint8(&rbuf, 0xAA);
  RBUF_WriteUint8(&rbuf, 0x01);
  RBUF_WriteUint8(&rbuf, 0x02);
  RBUF_WriteUint8(&rbuf, 0x03);

  // Read two bytes
  auto u8 = RBUF_ReadUint8(&rbuf);
  EXPECT_EQ(u8, 0xAA);
  u8 = RBUF_ReadUint8(&rbuf);
  EXPECT_EQ(u8, 0x01);

  RBUF_WriteUint8(&rbuf, 0x04);
  RBUF_WriteUint8(&rbuf, 0x05);

  u8 = RBUF_ReadUint8(&rbuf);
  EXPECT_EQ(u8, 0x02);
  u8 = RBUF_ReadUint8(&rbuf);
  EXPECT_EQ(u8, 0x03);
  u8 = RBUF_ReadUint8(&rbuf);
  EXPECT_EQ(u8, 0x04);
  u8 = RBUF_ReadUint8(&rbuf);
  EXPECT_EQ(u8, 0x05);

  EXPECT_EQ(rbuf.read_index, 1);
}
