#include "TestHarness.h"
#include "DgR2Matrix.h"
#include "DgR2Vector.h"

typedef Dg::R2::Matrix< float >     mat33;
typedef Dg::R2::Vector<float>        vec3;

//--------------------------------------------------------------------------------
//	Matrix Construction
//--------------------------------------------------------------------------------
TEST(Stack_Matrix33_Construction, creation_Matrix33_Construction)
{
  mat33 m0, m2;
  CHECK(m0.IsIdentity());

  float data[9] = {};
  m0.Set(data);
  CHECK(m0.IsZero());

  m0.Rotation(1.0f);

  mat33  m1(m0);
  CHECK(m1 == m0);

  m1 = m0;
  CHECK(m1 == m0);
  CHECK(!(m1 != m0));

  CHECK(!m1.IsZero());
  CHECK(!m1.IsIdentity());

  vec3 v0(1.0f, 1.25f, 1.75f);
  vec3 v1(2.0f, 2.25f, 2.75f);
  vec3 v2(3.0f, 3.25f, 3.75f);

  vec3 v4(1.0f, 2.0f, 4.0f);
  vec3 v5(1.25f, 2.25f, 4.25f);
  vec3 v6(1.5f, 2.5f, 4.5f);

  m0.SetRow(0, v0);
  m0.SetRow(1, v1);
  m0.SetRow(2, v2);

  m1.SetRows(v0, v1, v2);

  CHECK(m0 == m1);

  m0.SetColumn(0, Transpose(v0));
  m0.SetColumn(1, Transpose(v1));
  m0.SetColumn(2, Transpose(v2));

  m1.SetColumns(Transpose(v0), Transpose(v1), Transpose(v2));

  CHECK(m0 == m1);

  m0.GetRows(v0, v1, v2);

  m0.Clean();
  m0.Identity();
}


//--------------------------------------------------------------------------------
//	Matrix Inverse
//--------------------------------------------------------------------------------
TEST(Stack_Matrix33_Inverse, creation_Matrix33_Inverse)
{
  mat33 m0, m1, m2, ms, mr, mt;
  ms.Scaling(vec3(1.34f, 0.39f, 0.0f));
  mr.Rotation(0.234f);
  mt.Translation(vec3( 2.3f, 53.24f, 0.0f ));

  m0 = mt;
  m1 = m0.GetInverse();
  m2 = m1.GetInverse();
  CHECK(m2 == m0);

  m0 = mr;
  m1 = m0.GetInverse();
  m2 = m1.GetInverse();
  CHECK(m2 == m0);

  m0 = ms;
  m1 = m0.GetInverse();
  m2 = m1.GetInverse();
  CHECK(m2 == m0);

  m0 = mt * mr * ms;
  m1 = m0.GetInverse();
  m2 = m1.GetInverse();
  CHECK(m2 == m0);
}


//--------------------------------------------------------------------------------
//	Matrix VectorTransform
//--------------------------------------------------------------------------------
#include <iostream>
TEST(Stack_Matrix33_VectorTransform, creation_Matrix33_VectorTransform)
{
  {
    vec3 t0(0.5f, 0.5f, 0.0f);
    vec3 s0(0.5f, 0.5f, 0.0f);
    vec3 t1(0.4f, 0.4f, 0.0f); //0.2, 0.4
    vec3 s1(0.2f, 0.2f, 0.0f);

    mat33 mt0, mt1;

    {
      mat33 mt, ms;
      mt.Translation(t0);
      ms.Scaling(s0);
      mt0 = ms * mt;
    }

    {
      mat33 mt, ms;
      mt.Translation(t1);
      ms.Scaling(s1);
      mt1 = ms * mt;
    }

    mat33 mt2 = mt1 * mt0;
    mat33 mt3 = mt2.GetInverse();

    vec3 p0(0.0f, 0.0f, 1.0f);
    vec3 p1(1.0f, 1.0f, 1.0f);
    vec3 p0r = p0 * mt0;
    vec3 p1r = p1 * mt3;

    std::cout << "\np0: [" << p0[0] << ", " << p0[1] << "] -> [" << p0r[0] << ", " << p0r[1] << "]\n";
    std::cout << "p1: [" << p1[0] << ", " << p1[1] << "] -> [" << p1r[0] << ", " << p1r[1] << "]\n";
  }

  mat33 m, ms, mt, mr;

  ms.Scaling(2.0f);
  mr.Rotation(Dg::Constants<float>::PI / 2.0f);
  mt.Translation(vec3( 1.0f, 1.0f, 0.0f ));

  vec3 pin(2.0f, 3.0f, 1.0f);
  vec3 pout(-7.0f, 5.0f, 1.0f);
  vec3 result = pin * ms * mt * mr;
  CHECK(result == pout);
}