//! \file ut_rbuf_read_copy_block.cpp
//! \brief Ring rbuf unit test
//! \date  2023-04
//! \author Nicolas Boutin

#include <gtest/gtest.h>

extern "C" {
#include "ring_buffer/ring_buffer.h"
}

using namespace testing;

class RBUF_ReadCopyBlock_Fixture : public ::testing::Test
{
protected:
  void SetUp()
  {
    RBUF_InitEmpty(&rbuf, rbuf_data, DATA_SIZE);
  }
  // attributes
  RBUF_t rbuf;
  static constexpr uint8_t DATA_SIZE = 5;
  std::uint8_t rbuf_data[DATA_SIZE];
};

/**
 * \brief Bad input parameter
 */
TEST_F(RBUF_ReadCopyBlock_Fixture, read_copy_block_001)
{
  BUF_t buf;

  auto read = RBUF_ReadCopyBlock(nullptr, nullptr, 0);
  EXPECT_FALSE(read);

  read = RBUF_ReadCopyBlock(nullptr, nullptr, 1);
  EXPECT_FALSE(read);

  read = RBUF_ReadCopyBlock(nullptr, &rbuf, 0);
  EXPECT_FALSE(read);

  read = RBUF_ReadCopyBlock(nullptr, &rbuf, 1);
  EXPECT_FALSE(read);

  read = RBUF_ReadCopyBlock(&buf, nullptr, 0);
  EXPECT_FALSE(read);

  read = RBUF_ReadCopyBlock(&buf, nullptr, 1);
  EXPECT_FALSE(read);
}

/**
 * \brief Read empty buffer
 */
TEST_F(RBUF_ReadCopyBlock_Fixture, read_copy_block_002)
{
  BUF_t buf;
  uint8_t data[2] = {};
  BUF_InitEmpty(&buf, data, 2);

  auto res = RBUF_ReadCopyBlock(&buf, &rbuf, 2);
  EXPECT_FALSE(res);
  EXPECT_EQ(rbuf.read_index, 0);
}

/**
 * \brief Read one byte
 */
TEST_F(RBUF_ReadCopyBlock_Fixture, read_copy_block_003)
{
  // Put one byte in rbuf
  BUF_t buf_w;
  uint8_t data_w[1] = {0xAA};
  BUF_InitFull(&buf_w, data_w, 1);
  RBUF_WriteCopy(&rbuf, &buf_w, 1);

  // Read two bytes
  BUF_t buf_r;
  uint8_t data_r[1] = {0x00};
  BUF_InitEmpty(&buf_r, data_r, 1);
  auto res = RBUF_ReadCopyBlock(&buf_r, &rbuf, 1);
  EXPECT_TRUE(res);
  EXPECT_EQ(rbuf.read_index, 1);
  EXPECT_EQ(buf_r.data[0], 0xAA);
}

/**
 * \brief Read two bytes
 */
TEST_F(RBUF_ReadCopyBlock_Fixture, read_copy_block_004)
{
  // Put two bytes in rbuf
  BUF_t buf_w;
  uint8_t data_w[2] = {0xAA, 0xBB};
  BUF_InitFull(&buf_w, data_w, 2);
  RBUF_WriteCopy(&rbuf, &buf_w, 2);

  // Read two bytes
  BUF_t buf_r;
  uint8_t data_r[2] = {0x00, 0x00};
  BUF_InitEmpty(&buf_r, data_r, 2);
  auto res = RBUF_ReadCopyBlock(&buf_r, &rbuf, 2);
  EXPECT_TRUE(res);
  EXPECT_EQ(rbuf.read_index, 2);
  EXPECT_EQ(buf_r.data[0], 0xAA);
  EXPECT_EQ(buf_r.data[1], 0xBB);
}

/**
 * \brief Read with rollover
 */
TEST_F(RBUF_ReadCopyBlock_Fixture, read_copy_block_005)
{
  // Put two bytes in rbuf
  BUF_t buf_w;
  uint8_t data_w[2] = {0xAA, 0x01};
  BUF_InitFull(&buf_w, data_w, 2);
  RBUF_WriteCopy(&rbuf, &buf_w, 2);

  // Read two byte
  BUF_t buf_r;
  uint8_t data_r[2] = {};
  BUF_InitEmpty(&buf_r, data_r, 2);
  auto res = RBUF_ReadCopyBlock(&buf_r, &rbuf, 2);
  EXPECT_TRUE(res);
  EXPECT_EQ(rbuf.read_index, 2);
  EXPECT_EQ(buf_r.data[0], 0xAA);
  EXPECT_EQ(buf_r.data[1], 0x01);

  // Write 4 bytes
  uint8_t data_w2[4] = {0x02, 0x03, 0x04, 0x05};
  BUF_InitFull(&buf_w, data_w2, 4);
  RBUF_WriteCopy(&rbuf, &buf_w, 4);

  // Read 4 bytes
  uint8_t data_r2[4] = {};
  BUF_InitEmpty(&buf_r, data_r2, 4);
  res = RBUF_ReadCopyBlock(&buf_r, &rbuf, 4);
  EXPECT_TRUE(res);
  EXPECT_EQ(rbuf.read_index, 1);
  EXPECT_EQ(buf_r.data[0], 0x02);
  EXPECT_EQ(buf_r.data[1], 0x03);
  EXPECT_EQ(buf_r.data[2], 0x04);
  EXPECT_EQ(buf_r.data[3], 0x05);
}
