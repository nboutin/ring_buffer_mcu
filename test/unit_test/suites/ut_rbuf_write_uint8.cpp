//! \file ut_rbuf_write_uint8.cpp
//! \brief Ring rbuf unit test
//! \date  2023-04
//! \author Nicolas Boutin

#include <gtest/gtest.h>

extern "C" {
#include "ring_buffer/ring_buffer.h"
}

using namespace testing;

class RBUF_WriteUint8_Fixture : public ::testing::Test
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
 * \brief Add one byte to rbuf
 */
TEST_F(RBUF_WriteUint8_Fixture, write_uint8_001)
{
  auto res = RBUF_WriteUint8(&rbuf, 0xAA);
  EXPECT_EQ(res, true);
  EXPECT_EQ(rbuf.write_index, 1);
  EXPECT_EQ(rbuf.data[0], 0xAA);
}

/**
 * \brief Add two bytes to rbuf
 */
TEST_F(RBUF_WriteUint8_Fixture, write_uint8_002)
{
  auto res = RBUF_WriteUint8(&rbuf, 0xAA);
  EXPECT_EQ(res, true);
  EXPECT_EQ(rbuf.write_index, 1);
  EXPECT_EQ(rbuf.data[0], 0xAA);

  res = RBUF_WriteUint8(&rbuf, 0xBB);
  EXPECT_EQ(res, true);
  EXPECT_EQ(rbuf.write_index, 2);
  EXPECT_EQ(rbuf.data[1], 0xBB);
}

/**
 * \brief Write with rollover
 */
TEST_F(RBUF_WriteUint8_Fixture, write_uint8_003)
{
  // Put two bytes in rbuf
  RBUF_WriteUint8(&rbuf, 0x00);
  RBUF_WriteUint8(&rbuf, 0x01);

  // Read two bytes
  RBUF_ReadUint8(&rbuf);
  RBUF_ReadUint8(&rbuf);

  // Put for 4 bytes
  RBUF_WriteUint8(&rbuf, 0x02);
  RBUF_WriteUint8(&rbuf, 0x03);
  RBUF_WriteUint8(&rbuf, 0x04);
  RBUF_WriteUint8(&rbuf, 0x05);

  EXPECT_EQ(rbuf.write_index, 1);
  EXPECT_EQ(rbuf.read_index, 2);
  EXPECT_EQ(rbuf.data[0], 0x05);
  EXPECT_EQ(rbuf.data[1], 0x01);
  EXPECT_EQ(rbuf.data[2], 0x02);
  EXPECT_EQ(rbuf.data[3], 0x03);
  EXPECT_EQ(rbuf.data[4], 0x04);
}

/**
 * \brief Write on buffer full
 */
TEST_F(RBUF_WriteUint8_Fixture, write_uint8_004)
{
  // Put two bytes in rbuf
  bool res = RBUF_WriteUint8(&rbuf, 0x00);
  res &= RBUF_WriteUint8(&rbuf, 0x01);
  res &= RBUF_WriteUint8(&rbuf, 0x02);
  res &= RBUF_WriteUint8(&rbuf, 0x03);
  EXPECT_TRUE(res);

  res = RBUF_WriteUint8(&rbuf, 0x04);
  EXPECT_FALSE(res);

  EXPECT_EQ(rbuf.write_index, 4);
  EXPECT_EQ(rbuf.read_index, 0);
  EXPECT_EQ(rbuf.data[0], 0x00);
  EXPECT_EQ(rbuf.data[1], 0x01);
  EXPECT_EQ(rbuf.data[2], 0x02);
  EXPECT_EQ(rbuf.data[3], 0x03);
}
