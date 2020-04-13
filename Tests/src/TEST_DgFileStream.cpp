#include <vector>
#include <fstream>

#include "TestHarness.h"
#include "DgFileStream.h"

typedef Dg::FileStream stream;
typedef Dg::IO::byte byte;
typedef Dg::IO::myInt myInt;
typedef Dg::IO::ReturnType ReturnType;
typedef Dg::ErrorCode Err;
typedef std::vector<byte> byteVec;

#define FILE_NAME "DgFileStream-test.txt"
#define MAX_VAL 4

static uint32_t r = Dg::StreamOpenMode::read;
static uint32_t w = Dg::StreamOpenMode::write;
static uint32_t t = Dg::StreamOpenMode::trunc;

byteVec GetFileContents(std::string const & a_file)
{
  std::ifstream stream(a_file, std::ios::in | std::ios::binary);

  if (!stream.good())
    return byteVec();

  std::vector<uint8_t> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
  return contents;
}

void ResetFile(std::string const & a_file)
{
  std::ofstream fs(a_file, std::ios::binary);

  if (!fs.good())
    return;
  
  for (uint8_t i = 0; i <= MAX_VAL; i++)
    fs << i;
}

TEST(Stack_DgFileStream_r, creation_DgFileStream_r)
{
  ResetFile(FILE_NAME);
  stream fs(FILE_NAME, r);

  ReturnType result = fs.GetSize();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == MAX_VAL + 1);

  result = fs.GetPosition();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);

  CHECK(fs.IsOpen());
  CHECK(fs.IsReadable());
  CHECK(!fs.IsWritable());
  CHECK(fs.IsSeekable());

  result = fs.SetPosition(2);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 2);

  byte b;
  result = fs.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 2);

  result = fs.GetPosition();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 3);

  result = fs.WriteByte(1);
  CHECK(result.error == Dg::ErrorCode::Disallowed);
  CHECK(result.value == 0);

  fs.Close();
}

TEST(Stack_DgFileStream_w, creation_DgFileStream_w)
{
  ResetFile(FILE_NAME);
  stream fs(FILE_NAME, w);

  ReturnType result = fs.GetSize();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == MAX_VAL + 1);

  result = fs.GetPosition();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);

  CHECK(fs.IsOpen());
  CHECK(!fs.IsReadable());
  CHECK(fs.IsWritable());
  CHECK(fs.IsSeekable());

  byte b;
  result = fs.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::Disallowed);
  CHECK(result.value == 0);

  result = fs.WriteByte(1);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
}

TEST(Stack_DgFileStream_rw, creation_DgFileStream_rw)
{
  ResetFile(FILE_NAME);
  stream fs(FILE_NAME, w|r);

  ReturnType result = fs.GetSize();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == MAX_VAL + 1);

  result = fs.GetPosition();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);

  CHECK(fs.IsOpen());
  CHECK(fs.IsReadable());
  CHECK(fs.IsWritable());
  CHECK(fs.IsSeekable());

  byte b;
  result = fs.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 0);

  result = fs.WriteByte(7);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  fs.Close();

  byteVec v = GetFileContents(FILE_NAME);
  CHECK(v.size() == MAX_VAL + 1);
  CHECK(v[0] == 0);
  CHECK(v[1] == 7);
  CHECK(v[2] == 2);
  CHECK(v[3] == 3);
  CHECK(v[4] == 4);
}

TEST(Stack_DgFileStream_wt, creation_DgFileStream_wt)
{
  ResetFile(FILE_NAME);
  stream fs(FILE_NAME, w|t);

  ReturnType result = fs.GetSize();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);

  result = fs.GetPosition();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);

  CHECK(fs.IsOpen());
  CHECK(!fs.IsReadable());
  CHECK(fs.IsWritable());
  CHECK(fs.IsSeekable());

  byte b;
  fs.SetPosition(0);
  result = fs.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::Disallowed);
  CHECK(result.value == 0);

  result = fs.WriteByte(1);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
}

TEST(Stack_DgFileStream_rwt, creation_DgFileStream_rwt)
{
  ResetFile(FILE_NAME);
  stream fs(FILE_NAME, r|w|t);

  ReturnType result = fs.GetSize();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);

  result = fs.GetPosition();
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);

  CHECK(fs.IsOpen());
  CHECK(fs.IsReadable());
  CHECK(fs.IsWritable());
  CHECK(fs.IsSeekable());

  result = fs.WriteByte(1);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);

  byte b;
  fs.SetPosition(0);
  result = fs.ReadByte(b);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 1);
}

TEST(Stack_DgFileStream_Read, creation_DgFileStream_Read)
{
  ResetFile(FILE_NAME);
  stream fs(FILE_NAME, r);
  
  byte b;
  ReturnType result = fs.ReadByte(b);
  CHECK(fs.GetPosition().value == 1);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 0);

  CHECK(fs.ReadByte(b).value == 1);
  CHECK(fs.GetPosition().value == 2);
  CHECK(b == 1);
  
  CHECK(fs.ReadByte(b).value == 1);
  CHECK(fs.GetPosition().value == 3);
  CHECK(b == 2);

  CHECK(fs.ReadByte(b).value == 1);
  CHECK(fs.GetPosition().value == 4);
  CHECK(b == 3);

  CHECK(fs.ReadByte(b).value == 1);
  CHECK(fs.GetPosition().value == 5);
  CHECK(b == 4);

  result = fs.ReadByte(b);
  CHECK(fs.GetPosition().value == 5);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 0);
  CHECK(b == 4);
}

TEST(Stack_DgFileStream_Seek, creation_DgFileStream_Seek)
{
  ResetFile(FILE_NAME);
  stream fs(FILE_NAME, r);

  byte b;
  ReturnType result = fs.ReadByte(b);
  CHECK(fs.GetPosition().value == 1);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  CHECK(b == 0);

  CHECK(fs.Seek(1, Dg::StreamSeekOrigin::current).error == Dg::ErrorCode::None);
  CHECK(fs.ReadByte(b).value == 1);
  CHECK(fs.GetPosition().value == 3);
  CHECK(b == 2);

  CHECK(fs.Seek(-1, Dg::StreamSeekOrigin::current).error == Dg::ErrorCode::None);
  CHECK(fs.ReadByte(b).value == 1);
  CHECK(fs.GetPosition().value == 3);
  CHECK(b == 2);

  CHECK(fs.Seek(1, Dg::StreamSeekOrigin::begin).error == Dg::ErrorCode::None);
  CHECK(fs.ReadByte(b).value == 1);
  CHECK(fs.GetPosition().value == 2);
  CHECK(b == 1);

  CHECK(fs.Seek(-2, Dg::StreamSeekOrigin::end).error == Dg::ErrorCode::None);
  CHECK(fs.ReadByte(b).value == 1);
  CHECK(fs.GetPosition().value == 4);
  CHECK(b == 3);
}

TEST(Stack_DgFileStream_Write_1, creation_DgFileStream_Write_1)
{
  ResetFile(FILE_NAME);
  stream fs(FILE_NAME, w|r);

  fs.SetPosition(2);
  ReturnType result = fs.WriteByte(7);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 1);
  fs.Close();

  byteVec v = GetFileContents(FILE_NAME);
  CHECK(v.size() == MAX_VAL + 1);
  CHECK(v[0] == 0);
  CHECK(v[1] == 1);
  CHECK(v[2] == 7);
  CHECK(v[3] == 3);
  CHECK(v[4] == 4);
}


TEST(Stack_DgFileStream_Write_2, creation_DgFileStream_Write_2)
{
  ResetFile(FILE_NAME);
  stream fs(FILE_NAME, w|r);

  fs.SetPosition(2);

  byte buf[3] = {9, 8, 7};
  ReturnType result = fs.Write(buf, 3);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 3);
  fs.Close();

  byteVec v = GetFileContents(FILE_NAME);
  CHECK(v.size() == MAX_VAL + 1);
  CHECK(v[0] == 0);
  CHECK(v[1] == 1);
  CHECK(v[2] == 9);
  CHECK(v[3] == 8);
  CHECK(v[4] == 7);
}

TEST(Stack_DgFileStream_Write_3, creation_DgFileStream_Write_3)
{
  ResetFile(FILE_NAME);
  stream fs(FILE_NAME, w|r);

  fs.SetPosition(2);

  byte buf[5] = {9, 8, 7, 6, 5};
  ReturnType result = fs.Write(buf, 5);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 5);
  fs.Close();

  byteVec v = GetFileContents(FILE_NAME);
  CHECK(v.size() == MAX_VAL + 3);
  CHECK(v[0] == 0);
  CHECK(v[1] == 1);
  CHECK(v[2] == 9);
  CHECK(v[3] == 8);
  CHECK(v[4] == 7);
  CHECK(v[5] == 6);
  CHECK(v[6] == 5);
}

TEST(Stack_DgFileStream_Write_4, creation_DgFileStream_Write_4)
{
  ResetFile(FILE_NAME);
  stream fs(FILE_NAME, w|r);

  fs.SetPosition(7);

  byte buf[2] = {9, 8};
  ReturnType result = fs.Write(buf, 2);
  CHECK(result.error == Dg::ErrorCode::None);
  CHECK(result.value == 2);
  fs.Close();

  byteVec v = GetFileContents(FILE_NAME);
  CHECK(v.size() == MAX_VAL + 5);
  CHECK(v[0] == 0);
  CHECK(v[1] == 1);
  CHECK(v[2] == 2);
  CHECK(v[3] == 3);
  CHECK(v[4] == 4);
  CHECK(v[5] == 0);
  CHECK(v[6] == 0);
  CHECK(v[7] == 9);
  CHECK(v[8] == 8);
}