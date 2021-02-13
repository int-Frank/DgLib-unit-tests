#include "TestHarness.h"
#include "DgVector.h"

using namespace Dg;
typedef Vector2<float> vec2;

//--------------------------------------------------------------------------------
//	Vector Construction
//--------------------------------------------------------------------------------
TEST(Stack_Vector2catesian_Construction, creation_Vector2catesian_Construction)
{
  vec2 v0(1.0f, -4.0f);
  vec2 v1(v0);

  CHECK(v1 == v0);
  CHECK(!(v1 != v0));
  CHECK(!v0.IsZero());
  CHECK(!IsUnit(v0));

  vec2 p1;
  p1 = vec2(2.0f, -8.12f);
  p1.Clean();
  v0.Zero();
  v0 = v1;
  v0 = Normalize(v0);

  CHECK(IsUnit(v0));

  //Statics
  vec2 vec = Zeros2f();
  CHECK(vec[0] == 0.0f);
  CHECK(vec[1] == 0.0f);

  vec = Zeros2f();
  CHECK(vec[0] == 0.0f);
  CHECK(vec[1] == 0.0f);

  vec = xAxis2f();
  CHECK(vec[0] == 1.0f);
  CHECK(vec[1] == 0.0f);

  vec = yAxis2f();
  CHECK(vec[0] == 0.0f);
  CHECK(vec[1] == 1.0f);
}


//--------------------------------------------------------------------------------
//	Vector Accessors
//--------------------------------------------------------------------------------
TEST(Stack_Vector2catesian_Accessors, creation_Vector2catesian_Accessors)
{
  float x = -1.5f;
  float y = 4.2f;

  vec2 v(x, y);

  CHECK(v[0] == x);
  CHECK(v[1] == y);
}


//--------------------------------------------------------------------------------
//	Vector Arithmetic
//--------------------------------------------------------------------------------
TEST(Stack_Vector2catesian_Arithmetic, creation_Vector2catesian_Arithmetic)
{
  vec2 v0(1.6431f, 2.9012345f);
  vec2 v1(-0.032456f, 23.45809f);

  vec2 v2 = v0 + v1;
  v2 = v0 - v2;
  v2 = v2 * 3.4f;
  v2 = 1.2f * v2;
  v2 = -v0 + v2;
  v2 = v2 / 2.3f;
  v2 += v0;
  v2 -= v1;
  v2 *= 4.3f;
  v2 /= -3.8f;

  CHECK(v2 == vec2(-1.15278351f, 71.7770462f));
}


//--------------------------------------------------------------------------------
//	Vector Other functions
//--------------------------------------------------------------------------------
TEST(Stack_Vector2catesian_Other, creation_Vector2catesian_Other)
{
  // Dot ///////////////////////////////////////////////

  float r_Dot = Dot(vec2(2.0f, 0.0f), vec2(0.0f, 3.2f));
  CHECK(r_Dot == 0.0f);

  // Cross /////////////////////////////////////////////

  vec2 c0(1.0f, 1.0f);
  c0 = Normalize(c0);
  vec2 c1(Perpendicular(c0));

  float c2 = PerpDot(c0, c1);

  CHECK(Dg::IsZero(Dot(c0, c1)) && Dg::IsZero(1.0f - c2));

  // Length ///////////////////////////////////////////

  vec2 v(3.0f, 4.0f);

  float length = Mag(v);
  float lengthSquared = MagSq(v);

  CHECK(length == 5.0f);
  CHECK(lengthSquared == 25.0f);
}