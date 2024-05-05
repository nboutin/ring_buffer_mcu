//! \file ut_rbuf_init.cpp
//! \brief Ring rbuf init unit test
//! \date  2024-04
//! \author Nicolas Boutin

#include <gtest/gtest.h>

extern "C" {
#include "ring_buffer/ring_buffer.h"

extern const uint16_t BLOCK_FREE;

}

using namespace testing;

class RBUF_Init_Fixture : public ::testing::Test
{
protected:
  void SetUp()
  {}
  // attributes
  RBUF_t rbuf;
  static constexpr uint8_t DATA_SIZE = 5;
  std::uint8_t data[DATA_SIZE];
};

TEST_F(RBUF_Init_Fixture, init_001)
{
  RBUF_InitEmpty(&rbuf, data, DATA_SIZE);

  EXPECT_EQ(rbuf.size, DATA_SIZE);
  EXPECT_EQ(rbuf.write_index, 0);
  EXPECT_EQ(rbuf.read_index, 0);
}
