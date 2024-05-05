//! \file ut_rbuf_write_string.cpp
//! \brief Ring Buffer Unit Tests
//! \date  2024-04
//! \author Nicolas Boutin

#include <gmock/gmock.h>

extern "C" {
#include "ring_buffer/ring_buffer.h"
}

using namespace testing;
using testing::ElementsAreArray;

class RBUF_WriteString_UT : public ::testing::Test
{
protected:
  void SetUp()
  {
    RBUF_InitEmpty(&rbuf, data, DATA_SIZE);
  }
  // attributes
  RBUF_t rbuf;
  static constexpr uint8_t DATA_SIZE = 10;
  std::uint8_t data[DATA_SIZE];
};

/**
 * \brief Nominal case
 */
TEST_F(RBUF_WriteString_UT, WriteString_001)
{
  std::string hello = "Hello";

  auto res = RBUF_WriteString(&rbuf, hello.c_str(), hello.length());
  EXPECT_TRUE(res);

  std::vector<std::uint8_t> expected = {'H', 'e', 'l', 'l', 'o'};
  EXPECT_THAT(expected, ElementsAreArray(rbuf.data, hello.length()));
}

/**
 * \brief Nominal case with rollover
 */
TEST_F(RBUF_WriteString_UT, WriteString_002)
{
  memset(rbuf.data, 0x00, DATA_SIZE);
  rbuf.write_index = DATA_SIZE - 2;
  rbuf.read_index  = DATA_SIZE - 2;

  std::string hello = "Hello";
  auto res          = RBUF_WriteString(&rbuf, hello.c_str(), hello.length());
  EXPECT_TRUE(res);

  std::vector<std::uint8_t> expected = {'l', 'l', 'o', 0, 0, 0, 0, 0, 'H', 'e'};
  EXPECT_THAT(expected, ElementsAreArray(rbuf.data, expected.size()));
}

/**
 * \brief Bad input parameters
 */
TEST_F(RBUF_WriteString_UT, WriteString_003)
{
  std::string hello = "Hello";

  EXPECT_FALSE(RBUF_WriteString(nullptr, nullptr, 0));
  EXPECT_FALSE(RBUF_WriteString(nullptr, nullptr, 1));
  EXPECT_FALSE(RBUF_WriteString(nullptr, hello.c_str(), 0));
  EXPECT_FALSE(RBUF_WriteString(nullptr, hello.c_str(), 1));
  EXPECT_FALSE(RBUF_WriteString(&rbuf, nullptr, 0));
  EXPECT_FALSE(RBUF_WriteString(&rbuf, nullptr, 1));
  EXPECT_TRUE(RBUF_WriteString(&rbuf, hello.c_str(), 0));
}
