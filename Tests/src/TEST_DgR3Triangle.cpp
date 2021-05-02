#include "TestHarness.h"
#include "DgTriangle.h"
#include "DgQueryRayTriangle.h"

typedef double                  Real;
typedef Dg::Vector3<Real>    vec;
typedef Dg::Ray3<Real>       ray;
typedef Dg::Triangle3<Real>  triangle;

TEST(Stack_DgTriangle, DgTriangle)
{
  vec p0(1.0, -1.0, 0.0);
  vec p1(1.0, 1.0,  0.0);
  vec p2(1.0, 0.0, 1.0);
  triangle tri(p0, p1, p2);

  Dg::TI3RayTriangle<Real> tiRayTriangle;
  Dg::TI3RayTriangle<Real>::Result tiRayTriangle_res;

  vec ro(0.0, 0.0, -1.0);
  vec rd;

  ray r(ro, xAxis3d());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == false);

  r = ray(ro, -xAxis3d());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == false);

  r = ray(ro, yAxis3d());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == false);

  r = ray(ro, -yAxis3d());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == false);

  r = ray(ro, zAxis3d());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == false);

  r = ray(ro, -zAxis3d());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == false);

  r = ray(Zeros3d(), xAxis3d());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == true);
  CHECK(tiRayTriangle_res.u == 1.0);

  r = ray(vec(0.0, 0.0, 0.2), xAxis3d());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == true);
  CHECK(tiRayTriangle_res.u == 1.0);

  r = ray(vec(0.0, 0.0, 0.2), -xAxis3d());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == false);
}