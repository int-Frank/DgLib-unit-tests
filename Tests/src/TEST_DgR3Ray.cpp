#include "TestHarness.h"
#include "DgRay.h"
#include "DgLine.h"
#include "DgQueryPointRay.h"
#include "DgQueryRayLine.h"
#include "DgQueryRayRay.h"
#include "DgQueryRayPlane.h"

typedef double            Real;
typedef Dg::Vector3<Real> vec3;
typedef Dg::Plane<Real>   plane;
typedef Dg::Line3<Real>   line;
typedef Dg::Ray3<Real>    ray;

TEST(Stack_DgRayGeneral, DgRay)
{
  vec3 origin = Dg::Zeros<Real, 3>();
  vec3 direction = Dg::XAxis<Real, 3>();

  ray r0 = ray(origin, direction);

  CHECK(origin == r0.Origin() && direction == r0.Direction());
}

TEST(Stack_DgRayPoint, DgRay)
{
  vec3 origin = Dg::Zeros<Real, 3>();
  vec3 direction = Dg::XAxis<Real, 3>();
  ray r = ray(origin, direction);

  Dg::CP3PointRay<Real> dcpPointRay;
  Dg::CP3PointRay<Real>::Result dcpPointRay_res;
  vec3 pIn;

  //Point in front of ray
  pIn = vec3(7.0, -12.0, 35.0);
  dcpPointRay_res = dcpPointRay(pIn, r);
  CHECK(dcpPointRay_res.u == 7.0);
  CHECK(dcpPointRay_res.cp == vec3(7.0, 0.0, 0.0));

  //Point behind ray
  pIn = vec3(-2.0, -3.0, 6.0);
  dcpPointRay_res = dcpPointRay(pIn, r);
  CHECK(dcpPointRay_res.u == 0.0);
  CHECK(dcpPointRay_res.cp == r.Origin());
}

TEST(Stack_DgRayRay, DgRay)
{
  vec3 origin = Dg::Zeros<Real, 3>();
  vec3 direction = Dg::XAxis<Real, 3>();
  ray r0 = ray(origin, direction);
  ray r1;

  Dg::CP3RayRay<Real> dcpRayRay;
  Dg::CP3RayRay<Real>::Result dcpRayRay_res;

  //Rays parallel, no overlap, facing opposite directions
  r1.Set(vec3(-3.0, 8.0, 36.0), vec3(-1.0, 0.0, 0.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpRayRay_res.u0 == 0.0);
  CHECK(dcpRayRay_res.u1 == 0.0);
  CHECK(dcpRayRay_res.cp0 == r0.Origin());
  CHECK(dcpRayRay_res.cp1 == r1.Origin());

  //Rays parallel, overlap, facing opposite directions
  r1.Set(vec3(4.0, 13.0, 84.0), vec3(-1.0, 0.0, 0.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpRayRay_res.u0 == 0.0);
  CHECK(dcpRayRay_res.u1 == 4.0);
  CHECK(dcpRayRay_res.cp0 == r0.Origin());
  CHECK(dcpRayRay_res.cp1 == vec3(0.0, 13.0, 84.0));

  //Rays parallel, overlap, facing same direction
  r1.Set(vec3(4.0, 3.0, 4.0), vec3(1.0, 0.0, 0.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpRayRay_res.u0 == 4.0);
  CHECK(dcpRayRay_res.u1 == 0.0);
  CHECK(dcpRayRay_res.cp0 == vec3(4.0, 0.0, 0.0));
  CHECK(dcpRayRay_res.cp1 == r1.Origin());

  //Rays parallel, overlap, facing same direction, switch rays
  r1.Set(vec3(4.0, 3.0, 4.0), vec3(1.0, 0.0, 0.0));
  dcpRayRay_res = dcpRayRay(r1, r0);
  CHECK(dcpRayRay_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpRayRay_res.u1 == 4.0);
  CHECK(dcpRayRay_res.u0 == 0.0);
  CHECK(dcpRayRay_res.cp1 == vec3(4.0, 0.0, 0.0));
  CHECK(dcpRayRay_res.cp0 == r1.Origin());

  //Rays not parallel, cp are along r0, r1.Origin.
  r1.Set(vec3(3.0, 4.0, 3.0), vec3(0.0, 0.0, 1.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpRayRay_res.u0 == 3.0);
  CHECK(dcpRayRay_res.u1 == 0.0);
  CHECK(dcpRayRay_res.cp0 == vec3(3.0, 0.0, 0.0));
  CHECK(dcpRayRay_res.cp1 == r1.Origin());

  //Rays not parallel, cp are along both rays
  r1.Set(vec3(3.0, 4.0, 3.0), vec3(0.0, 0.0, -1.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpRayRay_res.u0 == 3.0);
  CHECK(dcpRayRay_res.u1 == 3.0);
  CHECK(dcpRayRay_res.cp0 == vec3(3.0, 0.0, 0.0));
  CHECK(dcpRayRay_res.cp1 == vec3(3.0, 4.0, 0.0));
}

TEST(Stack_DgRayLine, DgRay)
{
  vec3 origin = Dg::Zeros<Real, 3>();
  vec3 direction = Dg::XAxis<Real, 3>();
  ray r = ray(origin, direction);

  line ln;
  Dg::CP3RayLine<Real> dcpRayLine;
  Dg::CP3RayLine<Real>::Result dcpRayLine_res;

  //Line-Ray Parallel, same direction
  ln.Set(vec3(5.0, 11.0, 60.0), vec3(1.0, 0.0, 0.0));
  dcpRayLine_res = dcpRayLine(r, ln);
  CHECK(dcpRayLine_res.code == Dg::QueryCode::Parallel);
  CHECK(dcpRayLine_res.ur == 0.0);
  CHECK(dcpRayLine_res.ul == -5.0);
  CHECK(dcpRayLine_res.cpr == r.Origin());
  CHECK(dcpRayLine_res.cpl == vec3(0.0, 11.0, 60.0));

  //Line-Ray Parallel, opposite directions
  ln.Set(vec3(5.0, 11.0, 60.0), vec3(-1.0, 0.0, 0.0));
  dcpRayLine_res = dcpRayLine(r, ln);
  CHECK(dcpRayLine_res.code == Dg::QueryCode::Parallel);
  CHECK(dcpRayLine_res.ur == 0.0);
  CHECK(dcpRayLine_res.ul == 5.0);
  CHECK(dcpRayLine_res.cpr == r.Origin());
  CHECK(dcpRayLine_res.cpl == vec3(0.0, 11.0, 60.0));

  //Line-Ray not parallel, closest point is ray origin
  ln.Set(vec3(-9.0, 40.0, 61.0), vec3(0.0, 0.0, -1.0));
  dcpRayLine_res = dcpRayLine(r, ln);
  CHECK(dcpRayLine_res.code == Dg::QueryCode::Success);
  CHECK(dcpRayLine_res.ur == 0.0);
  CHECK(dcpRayLine_res.ul == 61.0);
  CHECK(dcpRayLine_res.cpr == r.Origin());
  CHECK(dcpRayLine_res.cpl == vec3(-9.0, 40.0, 0.0));

  //Line-Ray not parallel, closest point is along the ray
  ln.Set(vec3(9.0, 40.0, -61.0), vec3(0.0, 0.0, 1.0));
  dcpRayLine_res = dcpRayLine(r, ln);
  CHECK(dcpRayLine_res.code == Dg::QueryCode::Success);
  CHECK(dcpRayLine_res.ur == 9.0);
  CHECK(dcpRayLine_res.ul == 61.0);
  CHECK(dcpRayLine_res.cpr == vec3(9.0, 0.0, 0.0));
  CHECK(dcpRayLine_res.cpl == vec3(9.0, 40.0, 0.0));
}

TEST(Stack_DgRayPlane, DgRay)
{
  vec3 origin = Dg::Zeros<Real, 3>();
  vec3 direction = Dg::XAxis<Real, 3>();
  ray r = ray(origin, direction);

  //Ray-Plane
  Dg::TI3RayPlane<Real>          tiRayPlane;
  Dg::FI3RayPlane<Real>          fiRayPlane;
  Dg::TI3RayPlane<Real>::Result  tiRayPlane_res;
  Dg::FI3RayPlane<Real>::Result  fiRayPlane_res;
  plane pl;

  //Ray not intersecting plane, pointing away from plane
  pl.Set(vec3(-1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0));
  tiRayPlane_res = tiRayPlane(r, pl);
  CHECK(tiRayPlane_res.isIntersecting == false);

  fiRayPlane_res = fiRayPlane(r, pl);
  CHECK(fiRayPlane_res.code == Dg::QueryCode::NotIntersecting);
  CHECK(fiRayPlane_res.point == r.Origin());
  CHECK(fiRayPlane_res.u == 0.0);

  //Ray not intersecting plane, parallel to plane
  pl.Set(vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0));
  tiRayPlane_res = tiRayPlane(r, pl);
  CHECK(tiRayPlane_res.isIntersecting == false);

  fiRayPlane_res = fiRayPlane(r, pl);
  CHECK(fiRayPlane_res.code == Dg::QueryCode::NotIntersecting);
  CHECK(fiRayPlane_res.point == r.Origin());
  CHECK(fiRayPlane_res.u == 0.0);

  //Ray lies on the plane
  pl.Set(vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 0.0));
  tiRayPlane_res = tiRayPlane(r, pl);
  CHECK(tiRayPlane_res.isIntersecting == true);

  fiRayPlane_res = fiRayPlane(r, pl);
  CHECK(fiRayPlane_res.code == Dg::QueryCode::Overlapping);
  CHECK(fiRayPlane_res.point == r.Origin());
  CHECK(fiRayPlane_res.u == 0.0);

  //Ray intersects plane
  pl.Set(vec3(1.0, 0.0, 0.0), vec3(3.0, 0.0, 0.0));
  tiRayPlane_res = tiRayPlane(r, pl);
  CHECK(tiRayPlane_res.isIntersecting == true);

  fiRayPlane_res = fiRayPlane(r, pl);
  CHECK(fiRayPlane_res.code == Dg::QueryCode::Intersecting);
  CHECK(fiRayPlane_res.point == vec3(3.0, 0.0, 0.0));
  CHECK(fiRayPlane_res.u == 3.0);

  //Ray intersects plane
  r.Set(vec3(6.0, 2.3, -5.6), Dg::XAxis<Real, 3>());
  pl.Set(Dg::XAxis<Real, 3>(), vec3(10.0, 0.0, 0.0));
  tiRayPlane_res = tiRayPlane(r, pl);
  CHECK(tiRayPlane_res.isIntersecting == true);

  fiRayPlane_res = fiRayPlane(r, pl);
  CHECK(fiRayPlane_res.code == Dg::QueryCode::Intersecting);
  CHECK(fiRayPlane_res.point == vec3(10.0, 2.3, -5.6));
  CHECK(fiRayPlane_res.u == 4.0);

}