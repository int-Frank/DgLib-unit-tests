#include "TestHarness.h"
#include "DgPlane.h"
#include "DgQueryPointPlane.h"

typedef double Real;
typedef Dg::Vector3<Real>  vec3;
typedef Dg::Plane<Real>   plane;

TEST(Stack_DgPlane, DgPlane)
{
  // Construction and Setting
  vec3 normal(1.0, 0.0, 0.0);
  Real offset = -1.0;

  Real a = 4.0, b = 0.0, c = 0.0, d = -4.0;

  vec3 p0(1.0, 2.4, -5.6);
  vec3 p1(1.0, -7.9, 2.7);
  vec3 p2(1.0, 34.02, 11.6);

  plane pl0(normal, offset);
  plane pl1(a, b, c, d);
  plane pl2(p0, p2, p1);
  plane pl3(normal, p1);

  //Copying and assignment
  plane pl4(pl0);
  plane pl5 = pl0;

  //Returning internals
  vec3 n0 = pl1.Normal();
  Real o0 = pl1.Offset();
  vec3 n1;
  Real o1(0.0);

  //Distances
  vec3 p3(-1.0, 23.5, -90.4);

  CHECK(Dg::AreEqual(pl0.SignedDistance(p3), -2.0));
  CHECK(Dg::AreEqual(pl0.Distance(p3), 2.0));

  //Point-Plane query
  Dg::CP3PointPlane<Real>           dcpPointPlane;
  Dg::CP3PointPlane<Real>::Result   dcpPointPlane_res;

  dcpPointPlane_res = dcpPointPlane(p3, pl0);
  CHECK(dcpPointPlane_res.cp == vec3(1.0, 23.5, -90.4));
  CHECK(dcpPointPlane_res.distance == 2.0);
  CHECK(dcpPointPlane_res.signedDistance == -2.0);
}