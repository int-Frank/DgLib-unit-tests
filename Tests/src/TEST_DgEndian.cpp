#include "TestHarness.h"
#include "DgEndian.h"

TEST(Stack_Endian, creation_Endian)
{
  uint8_t ui8 = 0xAA;
  uint16_t ui16_b = 0xAABB;
  uint16_t ui16_l = 0xBBAA;
  uint32_t ui32_b = 0xAABBCCDD;
  uint32_t ui32_l = 0xDDCCBBAA;
  uint64_t ui64_b = 0x8899AABBCCDDEEFF;
  uint64_t ui64_l = 0xFFEEDDCCBBAA9988;

  int8_t i8 = static_cast<int8_t>(ui8);
  int16_t i16_b = static_cast<int16_t>(ui16_b);
  int16_t i16_l = static_cast<int16_t>(ui16_l);
  int32_t i32_b = static_cast<int32_t>(ui32_b);
  int32_t i32_l = static_cast<int32_t>(ui32_l);
  int64_t i64_b = static_cast<int64_t>(ui64_b);
  int64_t i64_l = static_cast<int64_t>(ui64_l);

  float f32_b = 0.0f;
  reinterpret_cast<uint8_t*>(&f32_b)[0] = 0xAA;
  reinterpret_cast<uint8_t*>(&f32_b)[1] = 0xBB;
  reinterpret_cast<uint8_t*>(&f32_b)[2] = 0xCC;
  reinterpret_cast<uint8_t*>(&f32_b)[3] = 0xDD;

  double f64_b = 0.0;
  reinterpret_cast<uint8_t*>(&f64_b)[0] = 0x88;
  reinterpret_cast<uint8_t*>(&f64_b)[1] = 0x99;
  reinterpret_cast<uint8_t*>(&f64_b)[2] = 0xAA;
  reinterpret_cast<uint8_t*>(&f64_b)[3] = 0xBB;
  reinterpret_cast<uint8_t*>(&f64_b)[4] = 0xCC;
  reinterpret_cast<uint8_t*>(&f64_b)[5] = 0xDD;
  reinterpret_cast<uint8_t*>(&f64_b)[6] = 0xEE;
  reinterpret_cast<uint8_t*>(&f64_b)[7] = 0xFF;

  float f32_l = 0.0f;
  reinterpret_cast<uint8_t*>(&f32_l)[3] = 0xAA;
  reinterpret_cast<uint8_t*>(&f32_l)[2] = 0xBB;
  reinterpret_cast<uint8_t*>(&f32_l)[1] = 0xCC;
  reinterpret_cast<uint8_t*>(&f32_l)[0] = 0xDD;

  double f64_l = 0.0;
  reinterpret_cast<uint8_t*>(&f64_l)[7] = 0x88;
  reinterpret_cast<uint8_t*>(&f64_l)[6] = 0x99;
  reinterpret_cast<uint8_t*>(&f64_l)[5] = 0xAA;
  reinterpret_cast<uint8_t*>(&f64_l)[4] = 0xBB;
  reinterpret_cast<uint8_t*>(&f64_l)[3] = 0xCC;
  reinterpret_cast<uint8_t*>(&f64_l)[2] = 0xDD;
  reinterpret_cast<uint8_t*>(&f64_l)[1] = 0xEE;
  reinterpret_cast<uint8_t*>(&f64_l)[0] = 0xFF;

  Dg::Endian thisEndian = Dg::Endianness();
  Dg::EndianConverter ec;

  ec.SetConversion(Dg::Endian::Big, Dg::Endian::Little);
  CHECK(ec.Convert<int8_t>(i8) == i8);
  CHECK(ec.Convert<int16_t>(i16_b) == i16_l);
  CHECK(ec.Convert<int32_t>(i32_b) == i32_l);
  CHECK(ec.Convert<int64_t>(i64_b) == i64_l);
  CHECK(ec.Convert<float>(f32_b) == f32_l);
  CHECK(ec.Convert<double>(f64_b) == f64_l);
  CHECK(ec.Convert<uint8_t>(ui8) == ui8);
  CHECK(ec.Convert<uint16_t>(ui16_b) == ui16_l);
  CHECK(ec.Convert<uint32_t>(ui32_b) == ui32_l);
  CHECK(ec.Convert<uint64_t>(ui64_b) == ui64_l);

  ec.SetConversion(Dg::Endian::Little, Dg::Endian::Big);
  CHECK(ec.Convert<int8_t>(i8) == i8);
  CHECK(ec.Convert<int16_t>(i16_l) == i16_b);
  CHECK(ec.Convert<int32_t>(i32_l) == i32_b);
  CHECK(ec.Convert<int64_t>(i64_l) == i64_b);
  CHECK(ec.Convert<float>(f32_l) == f32_b);
  CHECK(ec.Convert<double>(f64_l) == f64_b);
  CHECK(ec.Convert<uint8_t>(ui8) == ui8);
  CHECK(ec.Convert<uint16_t>(ui16_l) == ui16_b);
  CHECK(ec.Convert<uint32_t>(ui32_l) == ui32_b);
  CHECK(ec.Convert<uint64_t>(ui64_l) == ui64_b);

  ec.SetConversion(Dg::Endian::Little, Dg::Endian::Little);
  CHECK(ec.Convert<int8_t>(i8) == i8);
  CHECK(ec.Convert<int16_t>(i16_l) == i16_l);
  CHECK(ec.Convert<int32_t>(i32_l) == i32_l);
  CHECK(ec.Convert<int64_t>(i64_l) == i64_l);
  CHECK(ec.Convert<float>(f32_l) == f32_l);
  CHECK(ec.Convert<double>(f64_l) == f64_l);
  CHECK(ec.Convert<uint8_t>(ui8) == ui8);
  CHECK(ec.Convert<uint16_t>(ui16_l) == ui16_l);
  CHECK(ec.Convert<uint32_t>(ui32_l) == ui32_l);
  CHECK(ec.Convert<uint64_t>(ui64_l) == ui64_l);

  ec.SetConversion(Dg::Endian::Big, Dg::Endian::Big);
  CHECK(ec.Convert<int8_t>(i8) == i8);
  CHECK(ec.Convert<int16_t>(i16_b) == i16_b);
  CHECK(ec.Convert<int32_t>(i32_b) == i32_b);
  CHECK(ec.Convert<int64_t>(i64_b) == i64_b);
  CHECK(ec.Convert<float>(f32_b) == f32_b);
  CHECK(ec.Convert<double>(f64_b) == f64_b);
  CHECK(ec.Convert<uint8_t>(ui8) == ui8);
  CHECK(ec.Convert<uint16_t>(ui16_b) == ui16_b);
  CHECK(ec.Convert<uint32_t>(ui32_b) == ui32_b);
  CHECK(ec.Convert<uint64_t>(ui64_b) == ui64_b);

  ec.SetConversion(Dg::Endian::Unknown, Dg::Endian::Big);
  CHECK(ec.Convert<int8_t>(i8) == i8);
  CHECK(ec.Convert<int16_t>(i16_b) == i16_b);
  CHECK(ec.Convert<int32_t>(i32_b) == i32_b);
  CHECK(ec.Convert<int64_t>(i64_b) == i64_b);
  CHECK(ec.Convert<float>(f32_b) == f32_b);
  CHECK(ec.Convert<double>(f64_b) == f64_b);
  CHECK(ec.Convert<uint8_t>(ui8) == ui8);
  CHECK(ec.Convert<uint16_t>(ui16_b) == ui16_b);
  CHECK(ec.Convert<uint32_t>(ui32_b) == ui32_b);
  CHECK(ec.Convert<uint64_t>(ui64_b) == ui64_b);

  ec.SetConversion(Dg::Endian::Unknown, Dg::Endian::Little);
  CHECK(ec.Convert<int8_t>(i8) == i8);
  CHECK(ec.Convert<int16_t>(i16_b) == i16_b);
  CHECK(ec.Convert<int32_t>(i32_b) == i32_b);
  CHECK(ec.Convert<int64_t>(i64_b) == i64_b);
  CHECK(ec.Convert<float>(f32_b) == f32_b);
  CHECK(ec.Convert<double>(f64_b) == f64_b);
  CHECK(ec.Convert<uint8_t>(ui8) == ui8);
  CHECK(ec.Convert<uint16_t>(ui16_b) == ui16_b);
  CHECK(ec.Convert<uint32_t>(ui32_b) == ui32_b);
  CHECK(ec.Convert<uint64_t>(ui64_b) == ui64_b);

  ec.SetConversion(Dg::Endian::Big, Dg::Endian::Unknown);
  CHECK(ec.Convert<int8_t>(i8) == i8);
  CHECK(ec.Convert<int16_t>(i16_b) == i16_b);
  CHECK(ec.Convert<int32_t>(i32_b) == i32_b);
  CHECK(ec.Convert<int64_t>(i64_b) == i64_b);
  CHECK(ec.Convert<float>(f32_b) == f32_b);
  CHECK(ec.Convert<double>(f64_b) == f64_b);
  CHECK(ec.Convert<uint8_t>(ui8) == ui8);
  CHECK(ec.Convert<uint16_t>(ui16_b) == ui16_b);
  CHECK(ec.Convert<uint32_t>(ui32_b) == ui32_b);
  CHECK(ec.Convert<uint64_t>(ui64_b) == ui64_b);

  ec.SetConversion(Dg::Endian::Little, Dg::Endian::Unknown);
  CHECK(ec.Convert<int8_t>(i8) == i8);
  CHECK(ec.Convert<int16_t>(i16_b) == i16_b);
  CHECK(ec.Convert<int32_t>(i32_b) == i32_b);
  CHECK(ec.Convert<int64_t>(i64_b) == i64_b);
  CHECK(ec.Convert<float>(f32_b) == f32_b);
  CHECK(ec.Convert<double>(f64_b) == f64_b);
  CHECK(ec.Convert<uint8_t>(ui8) == ui8);
  CHECK(ec.Convert<uint16_t>(ui16_b) == ui16_b);
  CHECK(ec.Convert<uint32_t>(ui32_b) == ui32_b);
  CHECK(ec.Convert<uint64_t>(ui64_b) == ui64_b);

  ec.SetConversion(Dg::Endian::Unknown, Dg::Endian::Unknown);
  CHECK(ec.Convert<int8_t>(i8) == i8);
  CHECK(ec.Convert<int16_t>(i16_b) == i16_b);
  CHECK(ec.Convert<int32_t>(i32_b) == i32_b);
  CHECK(ec.Convert<int64_t>(i64_b) == i64_b);
  CHECK(ec.Convert<float>(f32_b) == f32_b);
  CHECK(ec.Convert<double>(f64_b) == f64_b);
  CHECK(ec.Convert<uint8_t>(ui8) == ui8);
  CHECK(ec.Convert<uint16_t>(ui16_b) == ui16_b);
  CHECK(ec.Convert<uint32_t>(ui32_b) == ui32_b);
  CHECK(ec.Convert<uint64_t>(ui64_b) == ui64_b);
}