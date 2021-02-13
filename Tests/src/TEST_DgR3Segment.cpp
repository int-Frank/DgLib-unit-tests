#include "TestHarness.h"
#include "DgSegment.h"
#include "DgPlane.h"
#include "DgLine.h"
#include "DgRay.h"
#include "DgMatrix.h"
#include "DgVQS.h"
#include "DgQueryPointSegment.h"
#include "DgQuerySegmentLine.h"
#include "DgQuerySegmentRay.h"
#include "DgQuerySegmentSegment.h"
#include "DgQuerySegmentPlane.h"

typedef double Real;
typedef Dg::Vector3<Real>             vec3;
typedef Dg::Plane<Real>               plane;
typedef Dg::Line3<Real>                line;
typedef Dg::Matrix44<Real>            mat44;
typedef Dg::VQS<Real>                 vqs;
typedef Dg::Quaternion<Real>          quat;
typedef Dg::Ray3<Real>                 ray;
typedef Dg::Segment3<Real>             lineSeg;

TEST(Stack_DgLineSegment, DgLineSegment)
{
  vec3 origin = Dg::Zeros<Real, 3>();
  vec3 direction = Dg::XAxis<Real, 3>() * 5.0;

  lineSeg ls0 = lineSeg(origin, direction);

  CHECK(Dg::AreEqual(ls0.Length(), 5.0));
  CHECK(Dg::AreEqual(ls0.LengthSquared(), 25.0));

  vec3 p0, p1;
  ls0.Get(p0, p1);
  CHECK((p0 == Dg::Zeros<Real, 3>()) && p1 == vec3(5.0, 0.0, 0.0));
  CHECK(ls0.GetP0() == (Dg::Zeros<Real, 3>()));
  CHECK(ls0.GetP1() == vec3(5.0, 0.0, 0.0));
  CHECK(ls0.GetCenter() == vec3(2.5, 0.0, 0.0));

  lineSeg ls1 = ls0;
  lineSeg ls2(ls1);
  CHECK(ls1 == ls0);
  CHECK(ls2 == ls0);

  //Geometric tests

  //lineSeg-point
  Dg::CP3PointSegment<Real>           dcpPointLS;
  Dg::CP3PointSegment<Real>::Result   dcpPointLS_res;
  p0 = vec3(2.0, 0.0, 0.0);
  lineSeg ls_zero(p0, p0);
  ls0.Set(vec3(2.0, 0.0, 0.0), vec3(6.0, 0.0, 0.0));
  vec3 pIn;

  //Point behind p0
  pIn = vec3(0.0, 3.0, -6.0);
  dcpPointLS_res = dcpPointLS(pIn, ls0);
  CHECK(dcpPointLS_res.u == 0.0);
  CHECK(dcpPointLS_res.cp == ls0.GetP0());
  //CHECK(dcpPointLS_res.distance == 7.0);
  //CHECK(dcpPointLS_res.sqDistance == 49.0);

  //Point behind p1
  pIn = vec3(8.0, 3.0, -6.0);
  dcpPointLS_res = dcpPointLS(pIn, ls0);
  CHECK(dcpPointLS_res.u == 1.0);
  CHECK(dcpPointLS_res.cp == ls0.GetP1());
  //CHECK(dcpPointLS_res.distance == 7.0);
  //CHECK(dcpPointLS_res.sqDistance == 49.0);

  //Closest point along lineSeg
  pIn = vec3(3.0, 3.0, -4.0);
  dcpPointLS_res = dcpPointLS(pIn, ls0);
  CHECK(dcpPointLS_res.u == 0.25);
  CHECK(dcpPointLS_res.cp == vec3(3.0, 0.0, 0.0));
  //CHECK(dcpPointLS_res.distance == 5.0);
  //CHECK(dcpPointLS_res.sqDistance == 25.0);

  //Point - zero length Segment
  pIn = vec3(3.0, 3.0, -4.0);
  dcpPointLS_res = dcpPointLS(pIn, ls_zero);
  CHECK(dcpPointLS_res.u == 1.0);
  CHECK(dcpPointLS_res.cp == p0);

  //lineSeg-Line
  Dg::CP3SegmentLine<Real>           dcpLSLine;
  Dg::CP3SegmentLine<Real>::Result   dcpLSLine_res;
  line l;

  //LineSeg parallel to line
  l.Set(vec3(3.0, 3.0, 4.0), Dg::XAxis<Real, 3>());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == Dg::QueryCode::Parallel);
  CHECK(dcpLSLine_res.ul == -1.0);
  CHECK(dcpLSLine_res.us == 0.0);
  CHECK(dcpLSLine_res.cps == ls0.GetP0());
  CHECK(dcpLSLine_res.cpl == vec3(2.0, 3.0, 4.0));
  //CHECK(dcpLSLine_res.distance == 5.0);
  //CHECK(dcpLSLine_res.sqDistance == 25.0);

  //LineSeg parallel to line, opposite direction
  l.Set(vec3(3.0, 3.0, 4.0), -Dg::XAxis<Real, 3>());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == Dg::QueryCode::Parallel);
  CHECK(dcpLSLine_res.ul == 1.0);
  CHECK(dcpLSLine_res.us == 0.0);
  CHECK(dcpLSLine_res.cps == ls0.GetP0());
  CHECK(dcpLSLine_res.cpl == vec3(2.0, 3.0, 4.0));
  //CHECK(dcpLSLine_res.distance == 5.0);
  //CHECK(dcpLSLine_res.sqDistance == 25.0);

  //lineSeg-p0 closest point
  l.Set(vec3(-1.0, 4.0, 3.0), -Dg::ZAxis<Real, 3>());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLine_res.ul == 3.0);
  CHECK(dcpLSLine_res.us == 0.0);
  CHECK(dcpLSLine_res.cps == ls0.GetP0());
  CHECK(dcpLSLine_res.cpl == vec3(-1.0, 4.0, 0.0));
  //CHECK(dcpLSLine_res.distance == 5.0);
  //CHECK(dcpLSLine_res.sqDistance == 25.0);

  //lineSeg-p1 closest point
  l.Set(vec3(9.0, 4.0, 3.0), -Dg::ZAxis<Real, 3>());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLine_res.ul == 3.0);
  CHECK(dcpLSLine_res.us == 1.0);
  CHECK(dcpLSLine_res.cps == ls0.GetP1());
  CHECK(dcpLSLine_res.cpl == vec3(9.0, 4.0, 0.0));
  //CHECK(dcpLSLine_res.distance == 5.0);
  //CHECK(dcpLSLine_res.sqDistance == 25.0);

  //Closest point along lineSeg
  l.Set(vec3(3.0, 4.0, 3.0), Dg::ZAxis<Real, 3>());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLine_res.ul == -3.0);
  CHECK(dcpLSLine_res.us == 0.25);
  CHECK(dcpLSLine_res.cps == vec3(3.0, 0.0, 0.0));
  CHECK(dcpLSLine_res.cpl == vec3(3.0, 4.0, 0.0));
  //CHECK(dcpLSLine_res.distance == 4.0);
  //CHECK(dcpLSLine_res.sqDistance == 16.0);

  //Line - zero length Segment
  l.Set(vec3(3.0, 4.0, 3.0), Dg::XAxis<Real, 3>());
  dcpLSLine_res = dcpLSLine(ls_zero, l);
  CHECK(dcpLSLine_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLine_res.ul == -1.0);
  CHECK(dcpLSLine_res.us == 0.0);
  CHECK(dcpLSLine_res.cps == p0);
  CHECK(dcpLSLine_res.cpl == vec3(2.0, 4.0, 3.0));

  //lineSeg-Ray
  ray r;
  Dg::CP3SegmentRay<Real>           dcpLSRay;
  Dg::CP3SegmentRay<Real>::Result   dcpLSRay_res;

  //LineSeg parallel to ray, but behind ray origin
  r.Set(vec3(-1.0, 4.0, 12.0), -Dg::XAxis<Real, 3>());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == r.Origin());
  //CHECK(dcpLSRay_res.distance == 13.0);
  //CHECK(dcpLSRay_res.sqDistance == 13.0 * 13.0);

  //LineSeg parallel to ray, but behind ray origin, switch LineSeg direction
  r.Set(vec3(9.0, 4.0, -12.0), Dg::XAxis<Real, 3>());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 1.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP1());
  CHECK(dcpLSRay_res.cpr == r.Origin());
  //CHECK(dcpLSRay_res.distance == 13.0);
  //CHECK(dcpLSRay_res.sqDistance == 13.0 * 13.0);

  //LineSeg parallel to ray, p0 behind ray origin, p1 along ray
  r.Set(vec3(4.0, 3.0, 4.0), Dg::XAxis<Real, 3>());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 0.5);
  CHECK(dcpLSRay_res.cps == vec3(4.0, 0.0, 0.0));
  CHECK(dcpLSRay_res.cpr == r.Origin());
  //CHECK(dcpLSRay_res.distance == 5.0);
  //CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg parallel to ray, p1 behind ray origin, p0 along ray
  r.Set(vec3(4.0, 3.0, 4.0), -Dg::XAxis<Real, 3>());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSRay_res.ur == 2.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == vec3(2.0, 3.0, 4.0));
  //CHECK(dcpLSRay_res.distance == 5.0);
  //CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg parallel to ray, completely in front of ray, p0 closer to ray origin
  r.Set(vec3(-2.0, 3.0, 4.0), Dg::XAxis<Real, 3>());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSRay_res.ur == 4.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls0.Origin());
  CHECK(dcpLSRay_res.cpr == vec3(2.0, 3.0, 4.0));
  //CHECK(dcpLSRay_res.distance == 5.0);
  //CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg parallel to ray, completely in front of ray, p1 closer to ray origin
  r.Set(vec3(10.0, 3.0, 4.0), -Dg::XAxis<Real, 3>());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSRay_res.ur == 8.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == vec3(2.0, 3.0, 4.0));
  //CHECK(dcpLSRay_res.distance == 5.0);
  //CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg not parallel, behind ray, Closest points are ls-p0, ray-origin
  r.Set(vec3(1.0, 6.0, -18.0), -Dg::ZAxis<Real, 3>());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == r.Origin());
  //CHECK(dcpLSRay_res.distance == 19.0);
  //CHECK(dcpLSRay_res.sqDistance == 19.0 * 19.0);
  
  //LineSeg not parallel, behind ray, Closest points are ls-p1, ray-origin
  r.Set(vec3(7.0, -6.0, 18.0), -Dg::YAxis<Real, 3>());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 1.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP1());
  CHECK(dcpLSRay_res.cpr == r.Origin());
  //CHECK(dcpLSRay_res.distance == 19.0);
  //CHECK(dcpLSRay_res.sqDistance == 19.0 * 19.0);
  
  //LineSeg not parallel, behind ray, Closest points are along the ls, ray-origin
  r.Set(vec3(5.0, -3.0, 4.0), -Dg::YAxis<Real, 3>());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 0.75);
  CHECK(dcpLSRay_res.cps == vec3(5.0, 0.0, 0.0));
  CHECK(dcpLSRay_res.cpr == r.Origin());
  //CHECK(dcpLSRay_res.distance == 5.0);
  //CHECK(dcpLSRay_res.sqDistance == 25.0);
  
  //LineSeg in front of ray, closest point p0
  r.Set(vec3(-1.0, -4.0, -3.0), Dg::ZAxis<Real, 3>());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 3.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == vec3(-1.0, -4.0, 0.0));
  //CHECK(dcpLSRay_res.distance == 5.0);
  //CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg in front of ray, closest point p1
  r.Set(vec3(9.0, -4.0, -3.0), Dg::ZAxis<Real, 3>());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 3.0);
  CHECK(dcpLSRay_res.us == 1.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP1());
  CHECK(dcpLSRay_res.cpr == vec3(9.0, -4.0, 0.0));
  //CHECK(dcpLSRay_res.distance == 5.0);
  //CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg in front of ray, closest point along ls
  r.Set(vec3(5.0, -4.0, -3.0), Dg::ZAxis<Real, 3>());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 3.0);
  CHECK(dcpLSRay_res.us == 0.75);
  CHECK(dcpLSRay_res.cps == vec3(5.0, 0.0, 0.0));
  CHECK(dcpLSRay_res.cpr == vec3(5.0, -4.0, 0.0));
  //CHECK(dcpLSRay_res.distance == 4.0);
  //CHECK(dcpLSRay_res.sqDistance == 16.0);

  //Ray- zero length Segment
  r.Set(vec3(3.0, 4.0, 3.0), Dg::XAxis<Real, 3>());
  dcpLSRay_res = dcpLSRay(ls_zero, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == p0);
  CHECK(dcpLSRay_res.cpr == vec3(3.0, 4.0, 3.0));


  //Ray- zero length Segment
  r.Set(vec3(1.0, 4.0, 3.0), Dg::XAxis<Real, 3>());
  dcpLSRay_res = dcpLSRay(ls_zero, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 1.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == p0);
  CHECK(dcpLSRay_res.cpr == vec3(2.0, 4.0, 3.0));

  //lineSeg-LineSeg
  Dg::CP3SegmentSegment<Real>           dcpLSLS;
  Dg::CP3SegmentSegment<Real>::Result   dcpLSLS_res;

  //Segmnets have zero length
  //ls1.Set(vec3(1.0, 4.0, 12.0), vec3(1.0, 4.0, 12.0));
  //ls2 = ls1;
  //dcpLSLS_res = dcpLSLS(ls1, ls2);
  //CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  //CHECK(dcpLSLS_res.u0 == 0.0);
  //CHECK(dcpLSLS_res.u1 == 0.0);
  //CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  //CHECK(dcpLSLS_res.cp1 == ls1.GetP0());

  //LineSegs parallel, no overlap, closest points ls0-p0, ls1-p0
  ls1.Set(vec3(-1.0, -4.0, 12.0), vec3(-5.0, -4.0, 12.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());
  //CHECK(dcpLSLS_res.distance == 13.0);
  //CHECK(dcpLSLS_res.sqDistance == 169.0);

  //LineSegs parallel, no overlap, closest points ls0-p0, ls1-p1
  ls1.Set(vec3(-5.0, -4.0, 12.0), vec3(-1.0, -4.0, 12.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 1.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP1());
  //CHECK(dcpLSLS_res.distance == 13.0);
  //CHECK(dcpLSLS_res.sqDistance == 169.0);
  
  //LineSegs parallel, no overlap, closest points ls0-p1, ls1-p0
  ls1.Set(vec3(9.0, -4.0, 12.0), vec3(18.0, -4.0, 12.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());
  //CHECK(dcpLSLS_res.distance == 13.0);
  //CHECK(dcpLSLS_res.sqDistance == 169.0);
  
  //LineSegs parallel, no overlap, closest points ls0-p1, ls1-p1
  ls1.Set(vec3(10.0, -4.0, 12.0), vec3(9.0, -4.0, 12.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 1.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP1());
  //CHECK(dcpLSLS_res.distance == 13.0);
  //CHECK(dcpLSLS_res.sqDistance == 169.0);
  
  //LineSegs parallel, overlap, ls0p0---ls1p0---ls0p1---ls1p1
  ls1.Set(vec3(4.0, -3.0, 4.0), vec3(10.0, -3.0, 4.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSLS_res.u0 == 0.5);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == vec3(4.0, 0.0, 0.0));
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());
  //CHECK(dcpLSLS_res.distance == 5.0);
  //CHECK(dcpLSLS_res.sqDistance == 25.0);

  //LineSegs parallel, overlap, ls0p1---ls1p0---ls0p0---ls1p1
  ls1.Set(vec3(4.0, -3.0, 4.0), vec3(0.0, -3.0, 4.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 0.5);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == vec3(2.0, -3.0, 4.0));
  //CHECK(dcpLSLS_res.distance == 5.0);
  //CHECK(dcpLSLS_res.sqDistance == 25.0);

  //LineSegs parallel, overlap, ls0p0---ls1p0---ls1p1---ls0p1
  ls1.Set(vec3(4.0, -3.0, 4.0), vec3(5.0, -3.0, 4.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSLS_res.u0 == 0.5);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == vec3(4.0, 0.0, 0.0));
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());
  //CHECK(dcpLSLS_res.distance == 5.0);
  //CHECK(dcpLSLS_res.sqDistance == 25.0);

  //LineSegs parallel, overlap, ls0p1---ls1p0---ls1p1---ls0p0
  ls1.Set(vec3(4.0, -3.0, 4.0), vec3(8.0, -3.0, 4.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSLS_res.u0 == 0.5);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == vec3(4.0, 0.0, 0.0));
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());
  //CHECK(dcpLSLS_res.distance == 5.0);
  //CHECK(dcpLSLS_res.sqDistance == 25.0);

  //LineSegs not parallel, closest points: ls0p0, ls1p0
  ls1.Set(vec3(-2.0, -5.0, 20.0), vec3(-2.0, -5.0, 23.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());
  //CHECK(dcpLSLS_res.distance == 21.0);
  //CHECK(dcpLSLS_res.sqDistance == 21.0 * 21.0);

  //LineSegs not parallel, closest points: ls0p0, ls1p1
  ls1.Set(vec3(-2.0, -5.0, 23.0), vec3(-2.0, -5.0, 20.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 1.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP1());
  //CHECK(dcpLSLS_res.distance == 21.0);
  //CHECK(dcpLSLS_res.sqDistance == 21.0 * 21.0);

  //LineSegs not parallel, closest points: ls0p1, ls1p0
  ls1.Set(vec3(10.0, -5.0, 20.0), vec3(10.0, -5.0, 23.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());
  //CHECK(dcpLSLS_res.distance == 21.0);
  //CHECK(dcpLSLS_res.sqDistance == 21.0 * 21.0);

  //LineSegs not parallel, closest points: ls0p1, ls1p1
  ls1.Set(vec3(10.0, -5.0, 23.0), vec3(10.0, -5.0, 20.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 1.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP1());
  //CHECK(dcpLSLS_res.distance == 21.0);
  //CHECK(dcpLSLS_res.sqDistance == 21.0 * 21.0);

  //LineSegs not parallel, closest points: ls0p0, ls1-along ls
  ls1.Set(vec3(-1.0, 4.0, -3.0), vec3(-1.0, 4.0, 3.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 0.5);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == vec3(-1.0, 4.0, 0.0));
  //CHECK(dcpLSLS_res.distance == 5.0);
  //CHECK(dcpLSLS_res.sqDistance == 25.0);

  //LineSegs not parallel, closest points: ls0p1, ls1-along ls
  ls1.Set(vec3(9.0, 4.0, -3.0), vec3(9.0, 4.0, 3.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 0.5);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == vec3(9.0, 4.0, 0.0));
  //CHECK(dcpLSLS_res.distance == 5.0);
  //CHECK(dcpLSLS_res.sqDistance == 25.0);

  //LineSegs not parallel, closest points: ls0-along ls, ls1-along ls
  ls1.Set(vec3(4.0, 4.0, -3.0), vec3(4.0, -4.0, -3.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 0.5);
  CHECK(dcpLSLS_res.u1 == 0.5);
  CHECK(dcpLSLS_res.cp0 == vec3(4.0, 0.0, 0.0));
  CHECK(dcpLSLS_res.cp1 == vec3(4.0, 0.0, -3.0));
  //CHECK(dcpLSLS_res.distance == 3.0);
  //CHECK(dcpLSLS_res.sqDistance == 9.0);

  //LineSeg-plane
  Dg::TI3SegmentPlane<Real>          tiLSPlane;
  Dg::FI3SegmentPlane<Real>          fiLSPlane;
  Dg::TI3SegmentPlane<Real>::Result  tiLSPlane_res;
  Dg::FI3SegmentPlane<Real>::Result  fiLSPlane_res;
  plane pl;
  
  //LineSeg parallel to plane
  pl.Set(Dg::YAxis<Real, 3>(), vec3(0.0, 2.0, 0.0));
  tiLSPlane_res = tiLSPlane(ls0, pl);
  CHECK(tiLSPlane_res.isIntersecting == false);

  fiLSPlane_res = fiLSPlane(ls0, pl);
  CHECK(fiLSPlane_res.code == Dg::QueryCode::NotIntersecting);
  CHECK(fiLSPlane_res.point == ls0.Origin());
  CHECK(fiLSPlane_res.u == 0.0);

  //LineSeg on plane
  pl.Set(Dg::YAxis<Real, 3>(), vec3(0.0, 0.0, 0.0));
  tiLSPlane_res = tiLSPlane(ls0, pl);
  CHECK(tiLSPlane_res.isIntersecting == true);

  fiLSPlane_res = fiLSPlane(ls0, pl);
  CHECK(fiLSPlane_res.code == Dg::QueryCode::Overlapping);
  CHECK(fiLSPlane_res.point == ls0.Origin());
  CHECK(fiLSPlane_res.u == 0.0);

  //LineSeg not parallel to plane, not intersecting, closest point p0
  pl.Set(vec3(-0.1, 1.0, 0.1), vec3(0.0, 3.0, 0.0));
  tiLSPlane_res = tiLSPlane(ls0, pl);
  CHECK(tiLSPlane_res.isIntersecting == false);

  fiLSPlane_res = fiLSPlane(ls0, pl);
  CHECK(fiLSPlane_res.code == Dg::QueryCode::NotIntersecting);
  CHECK(fiLSPlane_res.point == ls0.Origin());
  CHECK(fiLSPlane_res.u == 0.0);

  //LineSeg not parallel to plane, not intersecting, closest point p1
  pl.Set(vec3(0.1, 1.0, 0.1), vec3(0.0, 3.0, 0.0));
  tiLSPlane_res = tiLSPlane(ls0, pl);
  CHECK(tiLSPlane_res.isIntersecting == false);

  fiLSPlane_res = fiLSPlane(ls0, pl);
  CHECK(fiLSPlane_res.code == Dg::QueryCode::NotIntersecting);
  CHECK(fiLSPlane_res.point == ls0.GetP1());
  CHECK(fiLSPlane_res.u == 1.0);

  //LineSeg not parallel to plane, intersecting
  pl.Set(Dg::XAxis<Real, 3>(), vec3(4.0, 0.0, 0.0));
  tiLSPlane_res = tiLSPlane(ls0, pl);
  CHECK(tiLSPlane_res.isIntersecting == true);

  fiLSPlane_res = fiLSPlane(ls0, pl);
  CHECK(fiLSPlane_res.code == Dg::QueryCode::Intersecting);
  CHECK(fiLSPlane_res.point == vec3(4.0, 0.0, 0.0));
  CHECK(fiLSPlane_res.u == 0.5);

  //LineSeg not parallel to plane, intersecting, switch points
  pl.Set(-Dg::XAxis<Real, 3>(), vec3(4.0, 0.0, 0.0));
  tiLSPlane_res = tiLSPlane(ls0, pl);
  CHECK(tiLSPlane_res.isIntersecting == true);

  fiLSPlane_res = fiLSPlane(ls0, pl);
  CHECK(fiLSPlane_res.code == Dg::QueryCode::Intersecting);
  CHECK(fiLSPlane_res.point == vec3(4.0, 0.0, 0.0));
  CHECK(fiLSPlane_res.u == 0.5);
}