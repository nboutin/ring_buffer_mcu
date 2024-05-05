//! \file ut_rbuf.cpp
//! \brief Ring Buffer Unit Tests
//! \date  2024-04
//! \author Nicolas Boutin

#include <gtest/gtest.h>

extern "C" {
#include "ring_buffer/ring_buffer.h"
}

using namespace testing;

class RBUF_UT : public ::testing::Test
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
 * \brief Check write_index getter
 */
TEST_F(RBUF_UT, GetWriteIndex_001)
{
  EXPECT_EQ(RBUF_GetWriteIndex(&rbuf), 0);
  EXPECT_EQ(RBUF_GetWriteIndex(nullptr), 0);

  RBUF_WriteUint8(&rbuf, 0x01);
  EXPECT_EQ(RBUF_GetWriteIndex(&rbuf), 1);
  RBUF_WriteUint8(&rbuf, 0x01);
  EXPECT_EQ(RBUF_GetWriteIndex(&rbuf), 2);
}
