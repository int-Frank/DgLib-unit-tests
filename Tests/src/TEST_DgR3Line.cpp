#include "TestHarness.h"
#include "DgVector.h"
#include "DgMatrix44.h"
#include "DgLine.h"
#include "DgPlane.h"
#include "DgQueryLineLine.h"
#include "DgQueryPointLine.h"
#include "DgQueryLinePlane.h"

typedef double                    Real;
typedef Dg::Vector3<Real>         vec3;
typedef Dg::Plane<Real>           plane;
typedef Dg::Matrix44<Real>        mat44;
typedef Dg::VQS<Real>         vqs;
typedef Dg::Quaternion<Real>  quat;
typedef Dg::Line3<Real>           line;

TEST(Stack_DgLine, DgLine)
{
  vec3 origin = Dg::Zeros<Real, 3>();
  vec3 direction = Dg::XAxis<Real, 3>();

  line l0 = line(origin, direction);

  vec3 o, d;
  l0.Get(o, d);
  CHECK(o == l0.Origin() && d == l0.Direction());

  line l1 = l0;
  line l2(l1);
  CHECK(l1 == l0);
  CHECK(l2 == l0);

  //Geometric tests

  //Line-Point
  Dg::CP3PointLine<Real>          dcpPointLine;
  Dg::CP3PointLine<Real>::Result  dcpPointLine_res;

  dcpPointLine_res = dcpPointLine(vec3(7.0, -3.0, 4.0), l0);
  CHECK(dcpPointLine_res.u == 7.0);
  CHECK(dcpPointLine_res.cp == vec3(7.0, 0.0, 0.0));
  //CHECK(dcpPointLine_res.distance == 5.0);
  //CHECK(dcpPointLine_res.sqDistance == 25);

  //Line-Line
  Dg::CP3LineLine<Real>             dcpLineLine;
  Dg::CP3LineLine<Real>::Result     dcpLineLine_res;

  //Lines parallel
  l1.Set(vec3(1.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0));
  dcpLineLine_res = dcpLineLine(l0, l1);
  CHECK(dcpLineLine_res.code == Dg::QueryCode::Parallel);
  CHECK(dcpLineLine_res.cp0 == l0.Origin());
  CHECK(dcpLineLine_res.cp1 == vec3(0.0, 1.0, 0.0));
  //CHECK(dcpLineLine_res.sqDistance == 1.0);
  //CHECK(dcpLineLine_res.distance == 1.0);

  //Lines not parallel
  l1.Set(vec3(1.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));
  dcpLineLine_res = dcpLineLine(l0, l1);
  CHECK(dcpLineLine_res.code == Dg::QueryCode::Success);
  CHECK(dcpLineLine_res.cp0 == vec3(1.0, 0.0, 0.0));
  CHECK(dcpLineLine_res.cp1 == vec3(1.0, 1.0, 0.0));
  //CHECK(dcpLineLine_res.sqDistance == 1.0);
  //CHECK(dcpLineLine_res.distance == 1.0);

  //Line-Plane
  Dg::TI3LinePlane<Real>          tiLinePlane;
  Dg::FI3LinePlane<Real>          fiLinePlane;
  Dg::TI3LinePlane<Real>::Result  tiLinePlane_res;
  Dg::FI3LinePlane<Real>::Result  fiLinePlane_res;
  plane pl;

  //Line parallel to plane
  pl.Set(vec3(0.0, 1.0, 0.0), -1.0);
  tiLinePlane_res = tiLinePlane(l0, pl);
  CHECK(tiLinePlane_res.isIntersecting == false);

  fiLinePlane_res = fiLinePlane(l0, pl);
  CHECK(fiLinePlane_res.code == Dg::QueryCode::NotIntersecting);
  CHECK(fiLinePlane_res.point == l0.Origin());
  CHECK(fiLinePlane_res.u == 0.0);

  //Line on plane
  pl.Set(vec3(0.0, 1.0, 0.0), 0.0);
  tiLinePlane_res = tiLinePlane(l0, pl);
  CHECK(tiLinePlane_res.isIntersecting == true);

  fiLinePlane_res = fiLinePlane(l0, pl);
  CHECK(fiLinePlane_res.code == Dg::QueryCode::Overlapping);
  CHECK(fiLinePlane_res.point == l0.Origin());
  CHECK(fiLinePlane_res.u == 0.0);

  //Line intersecting plane
  pl.Set(vec3(1.0, 0.0, 0.0), -3.0);
  tiLinePlane_res = tiLinePlane(l0, pl);
  CHECK(tiLinePlane_res.isIntersecting == true);

  fiLinePlane_res = fiLinePlane(l0, pl);
  CHECK(fiLinePlane_res.code == Dg::QueryCode::Intersecting);
  CHECK(fiLinePlane_res.point == vec3(3.0, 0.0, 0.0));
  CHECK(fiLinePlane_res.u == 3.0);
}