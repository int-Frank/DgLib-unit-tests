#include "TestHarness.h"
#include "DgVector.h"
#include "DgGeometryAncillary.h"
#include "DgRNG_Global.h"

using namespace Dg;
typedef Vector3<float> vec3;

//--------------------------------------------------------------------------------
//	Vector Construction
//--------------------------------------------------------------------------------
TEST(Stack_Vector4_Construction, creation_Vector4_Construction)
{
  vec3 v0(1.0f, 2.0f, -4.0f);
  vec3 v1(v0);

  CHECK(v0.x() == 1.0f);
  CHECK(v0.y() == 2.0f);
  CHECK(v0.z() == -4.0f);

  CHECK(v1 == v0);
  CHECK(!(v1 != v0));
  CHECK(!v0.IsZero());
  CHECK(!IsUnit(v0));

  vec3 p1;
  p1 = vec3( 2.0f, 12.4f, -8.12f );
  p1.Clean();
  v0.Zero();
  v0 = v1;
  v0 = Normalize(v0);

  CHECK(IsUnit(v0));

  //Statics
  vec3 vec = Zeros3f();
  CHECK(vec[0] == 0.0f);
  CHECK(vec[1] == 0.0f);
  CHECK(vec[2] == 0.0f);

  vec = xAxis3f();
  CHECK(vec[0] == 1.0f);
  CHECK(vec[1] == 0.0f);
  CHECK(vec[2] == 0.0f);

  vec = xAxis3f();
  CHECK(vec[0] == 1.0f);
  CHECK(vec[1] == 0.0f);
  CHECK(vec[2] == 0.0f);

  vec = yAxis3f();
  CHECK(vec[0] == 0.0f);
  CHECK(vec[1] == 1.0f);
  CHECK(vec[2] == 0.0f);

  vec = zAxis3f();
  CHECK(vec[0] == 0.0f);
  CHECK(vec[1] == 0.0f);
  CHECK(vec[2] == 1.0f);

}


//--------------------------------------------------------------------------------
//	Vector Accessors
//--------------------------------------------------------------------------------
TEST(Stack_Vector4_Accessors, creation_Vector4_Accessors)
{
  float x = -1.5f;
  float y = 4.2f;
  float z = 9.0f;

  vec3 v( x, y, z );

  CHECK(v[0] == x);
  CHECK(v[1] == y);
  CHECK(v[2] == z);
}


//--------------------------------------------------------------------------------
//	Vector Arithmetic
//--------------------------------------------------------------------------------
TEST(Stack_Vector4_Arithmetic, creation_Vector4_Arithmetic)
{
  vec3 v0( 1.6431f, 2.9012345f, -4.0045f );
  vec3 v1( -0.032456f, 23.45809f, 13.375689f );

  vec3 v2 = v0 + v1;
  v2 = v0 - v2;
  v2 = v2 * 3.4f;
  v2 = 1.2f * v2;
  v2 = -v0 + v2;
  v2 = v2 / 2.3f;
  v2 += v0;
  v2 -= v1;
  v2 *= 4.3f;
  v2 /= -3.8f;

  CHECK(v2 == vec3( -1.15278351f, 71.7770462f, 44.5462036f ));
}


//--------------------------------------------------------------------------------
//	Vector Other functions
//--------------------------------------------------------------------------------
TEST(Stack_Vector4_Other, creation_Vector4_Other)
{
  // Dot ///////////////////////////////////////////////

  float r_Dot = Dot(vec3( 2.0f, 0.0f, 3.0f ), vec3( 0.0f, 3.2f, 0.0f ));
  CHECK(r_Dot == 0.0f);

  // Cross /////////////////////////////////////////////

  vec3 c0( 1.0f, 1.0f, 1.0f );
  c0 = Normalize(c0);
  vec3 c1(Perpendicular(c0));

  vec3 c2 = Cross(c0, c1);

  CHECK(Dg::IsZero(Dot(c0, c1)) &&
        Dg::IsZero(Dot(c0, c2)) &&
        Dg::IsZero(Dot(c1, c2)));

  // Length ///////////////////////////////////////////

  vec3 v( 12.34f, 89.002345f, -41.3785f );

  float length = Mag(v);
  float lengthSquared = MagSq(v);

  CHECK(length == 98.9235687f);
  CHECK(lengthSquared == 9785.87305f);

  // Random vectors ///////////////////////////////////

  v = Dg::GetRandomVector3<float>(Dg::RNG_Global());
  vec3 vs = Dg::GetRandomOrthonormalVector<float>(v, Dg::RNG_Global());

  CHECK(Dg::IsZero(Dot(v, vs)));

  v = Dg::GetRandomVector<float>(v, 10.3f, Dg::RNG_Global());

  // Basis ///////////////////////////////////////////////

  vec3 v0, v1, v2;

  Dg::GetBasis(vec3( 0.0f, 12.0f, 0.0f ),
               vec3(0.0f, 0.0f, 12.3f),
               v0, v1, v2);
  CHECK(v0 == vec3( 0.0f, 1.0f, 0.0f ));
  CHECK(v1 == vec3( 0.0f, 0.0f, 1.0f ));
  CHECK(v2 == vec3( 1.0f, 0.0f, 0.0f ));

  Dg::GetBasis(vec3(0.0f, 0.0f, 0.0f),
               vec3(0.0f, 0.0f, 0.0f),
               v0, v1, v2);
  CHECK(v0 == vec3(1.0f, 0.0f, 0.0f));
  CHECK(v1 == vec3(0.0f, 1.0f, 0.0f));
  CHECK(v2 == vec3(0.0f, 0.0f, 1.0f));
}