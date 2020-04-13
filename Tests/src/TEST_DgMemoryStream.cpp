#include "TestHarness.h"
#include "DgMemoryStream.h"

typedef Dg::MemoryStream stream;
typedef Dg::IO::byte byte;
typedef Dg::IO::myInt myInt;
typedef Dg::IO::ReturnType ReturnType;
typedef Dg::ErrorCode Err;

TEST(Stack_DgMemoryStream_0, creation_DgMemoryStream_0)
{
  stream ms;
  CHECK(ms.IsOpen());
  CHECK(ms.IsReadable());
  CHECK(ms.IsWritable());
  CHECK(ms.IsSeekable());

  ReturnType result = ms.GetSize();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);

  result;
  result = ms.WriteByte(1);  
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  result = ms.WriteByte(2);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  result = ms.WriteByte(3);  
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  result = ms.WriteByte(4);  
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  result = ms.WriteByte(5);  
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  CHECK(ms.GetSize().value == 5);

  ms.Seek(0, Dg::StreamSeekOrigin::begin);

  byte b;
  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 1);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 2);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 3);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 4);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 5);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);
  CHECK(b == 5);
}

TEST(Stack_DgMemoryStream_1, creation_DgMemoryStream_1)
{
  uint32_t r = Dg::StreamOpenMode::read;
  uint32_t w = Dg::StreamOpenMode::write;

  byte data[5] = {1, 2, 3, 4, 5};
  stream ms(5, data, r | w);
  CHECK(ms.IsOpen());
  CHECK(ms.IsReadable());
  CHECK(ms.IsWritable());
  CHECK(ms.IsSeekable());

  ReturnType result = ms.GetSize();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 5);

  ms.Seek(0, Dg::StreamSeekOrigin::begin);

  byte b;
  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 1);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 2);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 3);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 4);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 5);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);
  CHECK(b == 5);
}

TEST(Stack_DgMemoryStream_2, creation_DgMemoryStream_2)
{
  stream ms;
  ms.WriteByte(1);
  ms.Seek(2, Dg::StreamSeekOrigin::current);
  ms.WriteByte(4);
  
  ReturnType result = ms.GetSize();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 4);

  ms.Seek(0, Dg::StreamSeekOrigin::begin);
  byte b;
  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 1);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 0);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 0);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 4);

  ms.Seek(1, Dg::StreamSeekOrigin::begin);

  byte buf[4] = {2, 3, 4, 5};
  ms.Write(buf, 4);
  CHECK(ms.GetSize().value == 5);

  ms.Seek(0, Dg::StreamSeekOrigin::begin);
  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 1);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 2);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 3);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 4);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 5);

  result = ms.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);
  CHECK(b == 5);
}

TEST(Stack_DgMemoryStream_3, creation_DgMemoryStream_3)
{
  uint32_t r = Dg::StreamOpenMode::read;
  uint32_t w = Dg::StreamOpenMode::write;

  byte data[5] = {1, 2, 3, 4, 5};
  stream ms(5, data, r | w);

  byte buf[3] = {};
  ms.Seek(1, Dg::StreamSeekOrigin::begin);
  ms.Read(buf, 3);

  byte buf2[3] = {2, 3, 4};
  CHECK(memcmp(buf, buf2, 3) == 0);

  ms.Seek(-1, Dg::StreamSeekOrigin::end);
  ReturnType result = ms.Read(buf, 3);
  CHECK(result.value == 1);

  buf2[0] = 5;
  CHECK(memcmp(buf, buf2, 3) == 0);
}

TEST(Stack_DgMemoryStream_4, creation_DgMemoryStream_4)
{
  uint32_t r = Dg::StreamOpenMode::read;
  uint32_t w = Dg::StreamOpenMode::write;

  byte data[5] = {1, 2, 3, 4, 5};
  stream ms(5, data, r | w);

  ms.SetPosition(2);
  ReturnType result = ms.RemoveBlock(4);
  CHECK(result.error == Dg::ErrorCode::BadInput);

  result = ms.RemoveBlock(-4);
  CHECK(result.error == Dg::ErrorCode::BadInput);

  result = ms.RemoveBlock(3);
  CHECK(result.error == Dg::ErrorCode::None);

  result = ms.GetPosition();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 2);

  result = ms.GetSize();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 2);

  byte buf[2] = {};
  ms.Seek(0, Dg::StreamSeekOrigin::begin);
  result = ms.Read(buf, 2);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 2);

  byte buf2[2] = {1, 2};
  CHECK(memcmp(buf, buf2, 2) == 0);

  ms.Seek(0, Dg::StreamSeekOrigin::end);

  result = ms.RemoveBlock(-2);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);

  result = ms.GetPosition();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);

  result = ms.GetSize();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);
}

TEST(Stack_DgMemoryStream_5, creation_DgMemoryStream_5)
{
  uint32_t r = Dg::StreamOpenMode::read;
  uint32_t w = Dg::StreamOpenMode::write;

  stream ms(r | w);

  for (unsigned i = 0; i < 0xFFFF; i++)
    ms.WriteByte(byte(i));

  ms.SetPosition(0);

  byte b;
  for (unsigned i = 0; i < 0xFFFF; i++)
  {
    ReturnType result = ms.ReadByte(b);
    CHECK(result.error == Dg::ErrorCode::None);
    CHECK(result.value == 1);
    CHECK(b == byte(i));
  }
}

TEST(Stack_DgMemoryStream_6, creation_DgMemoryStream_6)
{
  uint32_t r = Dg::StreamOpenMode::read;
  uint32_t w = Dg::StreamOpenMode::write;

  stream ms0(r | w);
  stream ms1(r | w);

  for (unsigned i = 0; i < 0xFFFF; i++)
    ms0.WriteByte(byte(i));

  ms0.SetPosition(0);
  ms0.CopyTo(&ms1);
  ms1.SetPosition(0);

  byte b;
  for (unsigned i = 0; i < 0xFFFF; i++)
  {
    ReturnType result = ms1.ReadByte(b);
    CHECK(result.error == Dg::ErrorCode::None);
    CHECK(result.value == 1);
    CHECK(b == byte(i));
  }
}