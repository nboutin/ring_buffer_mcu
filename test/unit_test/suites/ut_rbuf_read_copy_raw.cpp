//! \file ut_rbuf_read_copy_raw.cpp
//! \brief Ring Buffer Unit Tests
//! \date  2024-04
//! \author Nicolas Boutin

#include <gmock/gmock.h>

extern "C" {
#include "ring_buffer/ring_buffer.h"
}

using namespace testing;
using testing::ElementsAreArray;

class RBUF_ReadCopyRaw_UT : public ::testing::Test
{
protected:
  void SetUp()
  {
    RBUF_InitEmpty(&rbuf, data, DATA_SIZE);
    BUF_InitEmpty(&buf, buf_data, DATA_SIZE);
  }
  // attributes
  static constexpr uint8_t DATA_SIZE = 10;

  RBUF_t rbuf;
  std::uint8_t data[DATA_SIZE];

  BUF_t buf;
  std::uint8_t buf_data[DATA_SIZE];
};

/**
 * \brief Nominal case
 */
TEST_F(RBUF_ReadCopyRaw_UT, ReadCopyRaw_001)
{
  std::vector<std::uint8_t> in_data{0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
  memcpy(rbuf.data, in_data.data(), in_data.size());
  rbuf.write_index = in_data.size();

  std::uint8_t to_read = 5;
  auto res             = RBUF_ReadCopyRaw(&buf, &rbuf, to_read);
  EXPECT_EQ(res, to_read);
  EXPECT_EQ(rbuf.read_index, to_read);
  std::vector<std::uint8_t> expected{0x00, 0x11, 0x22, 0x33, 0x44};
  EXPECT_THAT(expected, ElementsAreArray(buf.data, to_read));
}

/**
 * \brief Nominal case with rollover
 */
TEST_F(RBUF_ReadCopyRaw_UT, ReadCopyRaw_002)
{
  std::vector<std::uint8_t> in_data{0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
  memcpy(rbuf.data, in_data.data(), in_data.size());
  rbuf.write_index = 4;
  rbuf.read_index  = 6;

  std::uint8_t to_read = 6;
  auto res             = RBUF_ReadCopyRaw(&buf, &rbuf, to_read);
  EXPECT_EQ(res, to_read);
  EXPECT_EQ(rbuf.read_index, 2);
  std::vector<std::uint8_t> expected{0x66, 0x77, 0x88, 0x99, 0x00, 0x11};
  EXPECT_THAT(expected, ElementsAreArray(buf.data, to_read));
}

/**
 * \brief Bad input parameters
 */
TEST_F(RBUF_ReadCopyRaw_UT, ReadCopyRaw_003)
{
  EXPECT_EQ(RBUF_ReadCopyRaw(nullptr, nullptr, 0), 0);
  EXPECT_EQ(RBUF_ReadCopyRaw(nullptr, nullptr, 1), 0);
  EXPECT_EQ(RBUF_ReadCopyRaw(nullptr, &rbuf, 0), 0);
  EXPECT_EQ(RBUF_ReadCopyRaw(nullptr, &rbuf, 1), 0);
  EXPECT_EQ(RBUF_ReadCopyRaw(&buf, nullptr, 0), 0);
  EXPECT_EQ(RBUF_ReadCopyRaw(&buf, nullptr, 1), 0);
  EXPECT_EQ(RBUF_ReadCopyRaw(&buf, &rbuf, 0), 0);
}
