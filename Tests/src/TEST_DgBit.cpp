
#include <stdint.h>

#include "TestHarness.h"
#include "DgBit.h"

using namespace Dg;

TEST(Stack_bit_HighestBit, creation_bit_HighestBit)
{
  CHECK(Dg::HighestBit<uint32_t>(0) == 0);
  CHECK(Dg::HighestBit<uint32_t>(1) == 1);
  CHECK(Dg::HighestBit<uint32_t>(2) == 2);
  CHECK(Dg::HighestBit<uint32_t>(3) == 2);
  CHECK(Dg::HighestBit<uint32_t>(4) == 3);
  CHECK(Dg::HighestBit<uint32_t>(5) == 3);
  CHECK(Dg::HighestBit<uint32_t>(6) == 3);
  CHECK(Dg::HighestBit<uint32_t>(7) == 3);
  CHECK(Dg::HighestBit<uint32_t>(8) == 4);
  CHECK(Dg::HighestBit<uint32_t>(9) == 4);
  CHECK(Dg::HighestBit<uint32_t>(234) == 8);
  CHECK(Dg::HighestBit<uint32_t>(65535) == 16);
}

TEST(Stack_bit_BitManipulation, creation_bit_BitManipulation)
{
  uint32_t i(0);
  i = SetSubInt<uint32_t, 5, 13>(i, 7952);
  CHECK(i == 254464);
  CHECK((GetSubInt<uint32_t, 5, 13>(i)) == 7952);

  i = 0;
  i = SetSubInt<uint32_t, 0, 8>(i, 128);
  i = SetSubInt<uint32_t, 8, 8>(i, 56);
  i = SetSubInt<uint32_t, 16, 8>(i, 79);
  i = SetSubInt<uint32_t, 24, 8>(i, 211);

  CHECK((GetSubInt<uint32_t, 0, 8>(i)) == 128);
  CHECK((GetSubInt<uint32_t, 8, 8>(i)) == 56);
  CHECK((GetSubInt<uint32_t, 16, 8>(i)) == 79);
  CHECK((GetSubInt<uint32_t, 24, 8>(i)) == 211);
}

TEST(Stack_bit_BitManipulation_Runtime, creation_bit_BitManipulation_Runtime)
{
  uint32_t i(0);
  i = SetSubInt<uint32_t>(i, 7952, 5, 13);
  CHECK(i == 254464);
  CHECK((GetSubInt<uint32_t>(i, 5, 13)) == 7952);

  i = 0;
  i = SetSubInt<uint32_t>(i, 128, 0, 8);
  i = SetSubInt<uint32_t>(i, 56, 8, 8);
  i = SetSubInt<uint32_t>(i, 79, 16, 8);
  i = SetSubInt<uint32_t>(i, 211, 24, 8);

  CHECK((GetSubInt<uint32_t>(i, 0, 8)) == 128);
  CHECK((GetSubInt<uint32_t>(i, 8, 8)) == 56);
  CHECK((GetSubInt<uint32_t>(i, 16, 8)) == 79);
  CHECK((GetSubInt<uint32_t>(i, 24, 8)) == 211);
}