#include "TestHarness.h"
#include "DgVector.h"
#include "DgQuaternion.h"
#include "DgMatrix.h"
#include "DgVQS.h"

typedef Dg::Matrix44<double>    mat44;
typedef Dg::Quaternion<double>  quat;
typedef Dg::Vector3<double>     vec3;
typedef Dg::Vector4<double>     vec4;
typedef Dg::VQS<double>         VQS;

//--------------------------------------------------------------------------------
//	VQS Construction
//--------------------------------------------------------------------------------
TEST(Stack_VQS_Construction, creation_VQS_Construction)
{
  vec3 v(23.4, -84.2, 0.2134 );
  quat q;
  q.SetRotation(2.45, 0.8354, -1.345, Dg::EulerOrder::XYX);
  double s = 2.3;

  VQS vqs0, vqs3;
  VQS vqs1(v, q, s);
  vqs1.MakeValid();

  VQS vqs2(vqs1);
  CHECK(vqs2 == vqs1);
  CHECK(vqs2 != vqs0);

  vqs2 = vqs1;
  CHECK(vqs2 == vqs1);

  vqs2.Set(v, q, s);
  CHECK(vqs2 == vqs1);

  vqs2.V() = v;
  vqs2.Q() = q;
  vqs2.S() = s;
  CHECK(vqs2 == vqs1);

  vqs0.Identity();
  vqs2.Identity();
  vqs3.Identity();

  vqs0.V() = v;
  vqs2.Q() = q;
  vqs3.S() = s;
  CHECK((vqs3 * vqs2 * vqs0) == vqs1);

  vqs0.Identity();
  vqs2.Identity();
  vqs3.Identity();

  vqs0.V() = v;
  vqs2.Q() = q;
  vqs3.S() = s;
  vqs3 *= vqs2;
  vqs3 *= vqs0;
  CHECK(vqs3 == vqs1);
}


TEST(Stack_VQS_Inverse, creation_VQS_Inverse)
{
  vec3 v(23.4, -84.2, 0.2134);
  quat q;
  q.SetRotation(2.45, 0.8354, -1.345, Dg::EulerOrder::XYX);
  double s = 2.3;
  
  VQS vqs(v, q, s);
  VQS vqs_inv = vqs.GetInverse();

  VQS result = vqs * vqs_inv;
  CHECK(result == VQS());

  result = vqs_inv * vqs;
  CHECK(result == VQS());
}


TEST(Stack_VQS_VectorTransform, creation_VQS_VectorTransform)
{
  quat q;
  VQS T_A_WLD, T_B_A;
  mat44 M_A_WLD, M_B_A, mr, mt, ms;

  q.SetRotationY(-1.453);
  T_A_WLD.Set(vec3( 4.0, -2.56, 1.895 ), q, 1.534);
  mr.RotationY(-1.453);
  ms.Scaling(1.534);
  mt.Translation(vec3( 4.0, -2.56, 1.895 ));
  M_A_WLD = ms * mr * mt;

  q.SetRotationX(0.7473);
  T_B_A.Set(vec3( 1.73, -2.8567, 9.347 ), q, 2.747);
  mr.RotationX(0.7473);
  ms.Scaling(2.747);
  mt.Translation(vec3( 1.73, -2.8567, 9.347));
  M_B_A = ms * mr * mt;

  vec3 result_VQS, result_M;
  result_VQS = (T_B_A * T_A_WLD).TransformVector(vec3( 2.0, 0.0, 0.0 ));
  result_M = vec4( 2.0, 0.0, 0.0, 0.0 ) * (M_B_A * M_A_WLD);

  CHECK(result_VQS == result_M);

}