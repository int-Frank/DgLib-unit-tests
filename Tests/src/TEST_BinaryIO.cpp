#include "TestHarness.h"
#include "DgBinaryReader.h"
#include "DgBinaryWriter.h"
#include "DgMemoryStream.h"

typedef Dg::IO::byte byte;
typedef Dg::IO::myInt myInt;
typedef Dg::IO::ReturnType ReturnType;
typedef Dg::ErrorCode Err;

TEST(Stack_BinaryIO, creation_BinaryIO)
{
  int8_t const s8 = 0x1A;
  int16_t const s16 = 0x3BCD;
  int32_t const s32 = 0x1A2B3C4D;
  int64_t const s64 = 0x9684736251403F2ELL;
  uint8_t const u8 = 0xAD;
  uint16_t const u16 = 0x1357;
  uint32_t const u32 = 0x24681012;
  uint64_t const u64 = 0xFEDCBA9876543210LL;
  float const f32 = 1234.567891011121314151617181920f;
  double const f64 = -3.893758307507e131;
  std::string const str = "Hello world!";

  Dg::BinaryReader reader;
  Dg::BinaryWriter writer;

  Dg::MemoryStream stream(Dg::StreamOpenMode::read | Dg::StreamOpenMode::write);
  Dg::EndianConverter ec;

  ec.SetConversion(Dg::Endianness(), Dg::Endianness());

  writer.Open(&stream);
  writer.SetEndianConverter(ec);

  ReturnType result = writer.Write<int8_t>(&s8);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  result = writer.Write<int16_t>(&s16);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  result = writer.Write<int32_t>(&s32);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  result = writer.Write<int64_t>(&s64);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  result = writer.Write<uint8_t>(&u8);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  result = writer.Write<uint16_t>(&u16);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  result = writer.Write<uint32_t>(&u32);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  result = writer.Write<uint64_t>(&u64);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  result = writer.Write<float>(&f32);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  result = writer.Write<double>(&f64);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  result = writer.Write_string(str);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == str.length());

  writer.Close();
  stream.SetPosition(0);

  reader.Open(&stream);
  reader.SetEndianConverter(ec);

  int8_t ts8(0);
  int16_t ts16(0);
  int32_t ts32(0);
  int64_t ts64(0);
  uint8_t tu8(0);
  uint16_t tu16(0);
  uint32_t tu32(0);
  uint64_t tu64(0);
  float tf32(0.f);
  double tf64(0.0);
  std::string tstr;

  result = reader.Read<int8_t>(&ts8);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(ts8 == s8);

  result = reader.Read<int16_t>(&ts16);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(ts16 == s16);

  result = reader.Read<int32_t>(&ts32);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(ts32 == s32);

  result = reader.Read<int64_t>(&ts64);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(ts64 == s64);

  result = reader.Read<uint8_t>(&tu8);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(tu8 == u8);

  result = reader.Read<uint16_t>(&tu16);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(tu16 == u16);

  result = reader.Read<uint32_t>(&tu32);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(tu32 == u32);

  result = reader.Read<uint64_t>(&tu64);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(tu64 == u64);

  result = reader.Read<float>(&tf32);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(tf32 == f32);

  result = reader.Read<double>(&tf64);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(tf64 == f64);

  result = reader.Read_string(&tstr, str.length());
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == str.length());
  CHECK(tstr == str);
}