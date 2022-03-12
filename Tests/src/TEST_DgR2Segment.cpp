#include "TestHarness.h"
#include "DgSegment.h"
#include "DgRay.h"
#include "DgLine.h"
#include "DgVector.h"
#include "DgQueryPointSegment.h"
#include "DgQuerySegmentSegment.h"
#include "DgQuerySegmentRay.h"
#include "DgQuerySegmentLine.h"

typedef double Real;
typedef Dg::Vector2<Real>   vec2;
typedef Dg::Segment2<Real>  seg;
typedef Dg::Ray2<Real>      ray;
typedef Dg::Line2<Real>     line;

TEST(Stack_DgR2SegmentGeneral, DgSegment2)
{
  vec2 origin = Dg::Zeros<Real, 2>();
  vec2 direction = Dg::XAxis<Real, 2>() * 5.0;

  seg ls = seg(origin, direction);

  CHECK(Dg::AreEqual(ls.Length(), 5.0));
  CHECK(Dg::AreEqual(ls.LengthSquared(), 25.0));

  vec2 p0, p1;
  ls.Get(p0, p1);
  CHECK((p0 == Dg::Zeros<Real, 2>()) && (p1 == vec2(5.0, 0.0)));
  CHECK(ls.GetP0() == (Dg::Zeros<Real, 2>()));
  CHECK(ls.GetP1() == vec2(5.0, 0.0));
  CHECK(ls.GetCenter() == vec2(2.5, 0.0));
}

TEST(Stack_DgR2SegmentPoint, DgR2Segment)
{
  seg ls;
  ls.Set(vec2(2.0, 0.0), vec2(6.0, 0.0));
  Dg::CP2PointSegment<Real>           dcpPointLS;
  Dg::CP2PointSegment<Real>::Result   dcpPointLS_res;
  vec2 pIn;

  //Point behind p0
  pIn = vec2(0.0, 3.0);
  dcpPointLS_res = dcpPointLS(pIn, ls);
  CHECK(dcpPointLS_res.u == 0.0);
  CHECK(dcpPointLS_res.cp == ls.GetP0());

  //Point behind p1
  pIn = vec2(8.0, 3.0);
  dcpPointLS_res = dcpPointLS(pIn, ls);
  CHECK(dcpPointLS_res.u == 1.0);
  CHECK(dcpPointLS_res.cp == ls.GetP1());

  //Closest point along seg
  pIn = vec2(3.0, 3.0);
  dcpPointLS_res = dcpPointLS(pIn, ls);
  CHECK(dcpPointLS_res.u == 0.25);
  CHECK(dcpPointLS_res.cp == vec2(3.0, 0.0));
}

TEST(Stack_DgR2SegmentLine, DgR2Segment)
{
  seg ls;
  ls.Set(vec2(2.0, 0.0), vec2(6.0, 0.0));

  Dg::CP2SegmentLine<Real>           dcpLSLine;
  Dg::CP2SegmentLine<Real>::Result   dcpLSLine_res;
  line l;

  //LineSeg parallel to line
  l.Set(vec2(3.0, 3.0), Dg::XAxis<Real, 2>());
  dcpLSLine_res = dcpLSLine(ls, l);
  CHECK(dcpLSLine_res.code == Dg::QueryCode::Parallel);
  CHECK(dcpLSLine_res.ul == -1.0);
  CHECK(dcpLSLine_res.us == 0.0);
  CHECK(dcpLSLine_res.cps == ls.GetP0());
  CHECK(dcpLSLine_res.cpl == vec2(2.0, 3.0));

  //LineSeg parallel to line, opposite direction
  l.Set(vec2(3.0, 3.0), -Dg::XAxis<Real, 2>());
  dcpLSLine_res = dcpLSLine(ls, l);
  CHECK(dcpLSLine_res.code == Dg::QueryCode::Parallel);
  CHECK(dcpLSLine_res.ul == 1.0);
  CHECK(dcpLSLine_res.us == 0.0);
  CHECK(dcpLSLine_res.cps == ls.GetP0());
  CHECK(dcpLSLine_res.cpl == vec2(2.0, 3.0));

  //seg-p0 closest point
  l.Set(vec2(-1.0, 4.0), -Dg::YAxis<Real, 2>());
  dcpLSLine_res = dcpLSLine(ls, l);
  CHECK(dcpLSLine_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLine_res.ul == 4.0);
  CHECK(dcpLSLine_res.us == 0.0);
  CHECK(dcpLSLine_res.cps == ls.GetP0());
  CHECK(dcpLSLine_res.cpl == vec2(-1.0, 0.0));

  //seg-p1 closest point
  l.Set(vec2(9.0, 4.0), -Dg::YAxis<Real, 2>());
  dcpLSLine_res = dcpLSLine(ls, l);
  CHECK(dcpLSLine_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLine_res.ul == 4.0);
  CHECK(dcpLSLine_res.us == 1.0);
  CHECK(dcpLSLine_res.cps == ls.GetP1());
  CHECK(dcpLSLine_res.cpl == vec2(9.0, 0.0));

  //Intersecting
  l.Set(vec2(3.0, 4.0), -Dg::YAxis<Real, 2>());
  dcpLSLine_res = dcpLSLine(ls, l);
  CHECK(dcpLSLine_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLine_res.ul == 4.0);
  CHECK(dcpLSLine_res.us == 0.25);
  CHECK(dcpLSLine_res.cps == vec2(3.0, 0.0));
  CHECK(dcpLSLine_res.cpl == vec2(3.0, 0.0));
}

TEST(Stack_DgR2SegmentRay, DgR2Segment)
{
  seg ls;
  ls.Set(vec2(2.0, 0.0), vec2(6.0, 0.0));

  ray r;
  Dg::CP2SegmentRay<Real>           dcpLSRay;
  Dg::CP2SegmentRay<Real>::Result   dcpLSRay_res;

  //LineSeg parallel to ray, but behind ray origin
  r.Set(vec2(-1.0, 4.0), -Dg::XAxis<Real, 2>());
  dcpLSRay_res = dcpLSRay(ls, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls.GetP0());
  CHECK(dcpLSRay_res.cpr == r.Origin());

  //LineSeg parallel to ray, but behind ray origin, switch LineSeg direction
  r.Set(vec2(9.0, 4.0), Dg::XAxis<Real, 2>());
  dcpLSRay_res = dcpLSRay(ls, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 1.0);
  CHECK(dcpLSRay_res.cps == ls.GetP1());
  CHECK(dcpLSRay_res.cpr == r.Origin());

  //LineSeg parallel to ray, p0 behind ray origin, p1 along ray
  r.Set(vec2(4.0, 3.0), Dg::XAxis<Real, 2>());
  dcpLSRay_res = dcpLSRay(ls, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 0.5);
  CHECK(dcpLSRay_res.cps == vec2(4.0, 0.0));
  CHECK(dcpLSRay_res.cpr == r.Origin());

  //LineSeg parallel to ray, p1 behind ray origin, p0 along ray
  r.Set(vec2(4.0, 3.0), -Dg::XAxis<Real, 2>());
  dcpLSRay_res = dcpLSRay(ls, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSRay_res.ur == 2.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls.GetP0());
  CHECK(dcpLSRay_res.cpr == vec2(2.0, 3.0));

  //LineSeg parallel to ray, completely in front of ray, p0 closer to ray origin
  r.Set(vec2(-2.0, 3.0), Dg::XAxis<Real, 2>());
  dcpLSRay_res = dcpLSRay(ls, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSRay_res.ur == 4.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls.Origin());
  CHECK(dcpLSRay_res.cpr == vec2(2.0, 3.0));

  //LineSeg parallel to ray, completely in front of ray, p1 closer to ray origin
  r.Set(vec2(10.0, 3.0), -Dg::XAxis<Real, 2>());
  dcpLSRay_res = dcpLSRay(ls, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSRay_res.ur == 8.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls.GetP0());
  CHECK(dcpLSRay_res.cpr == vec2(2.0, 3.0));

  //LineSeg not parallel, behind ray, Closest points are ls-p0, ray-origin
  r.Set(vec2(1.0, 6.0), -Dg::XAxis<Real, 2>());
  dcpLSRay_res = dcpLSRay(ls, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls.GetP0());
  CHECK(dcpLSRay_res.cpr == r.Origin());

  //LineSeg not parallel, behind ray, Closest points are ls-p1, ray-origin
  r.Set(vec2(7.0, -6.0), -Dg::YAxis<Real, 2>());
  dcpLSRay_res = dcpLSRay(ls, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 1.0);
  CHECK(dcpLSRay_res.cps == ls.GetP1());
  CHECK(dcpLSRay_res.cpr == r.Origin());

  //LineSeg not parallel, behind ray, Closest points are along the ls, ray-origin
  r.Set(vec2(5.0, -3.0), -Dg::YAxis<Real, 2>());
  dcpLSRay_res = dcpLSRay(ls, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 0.75);
  CHECK(dcpLSRay_res.cps == vec2(5.0, 0.0));
  CHECK(dcpLSRay_res.cpr == r.Origin());

  //LineSeg in front of ray, closest point p0
  r.Set(vec2(-1.0, -4.0), Dg::YAxis<Real, 2>());
  dcpLSRay_res = dcpLSRay(ls, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 4.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls.GetP0());
  CHECK(dcpLSRay_res.cpr == vec2(-1.0, 0.0));

  //LineSeg in front of ray, closest point p1
  r.Set(vec2(9.0, -4.0), Dg::YAxis<Real, 2>());
  dcpLSRay_res = dcpLSRay(ls, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 4.0);
  CHECK(dcpLSRay_res.us == 1.0);
  CHECK(dcpLSRay_res.cps == ls.GetP1());
  CHECK(dcpLSRay_res.cpr == vec2(9.0, 0.0));

  //LineSeg in front of ray, closest point along ls
  r.Set(vec2(5.0, -4.0), Dg::YAxis<Real, 2>());
  dcpLSRay_res = dcpLSRay(ls, r);
  CHECK(dcpLSRay_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSRay_res.ur == 4.0);
  CHECK(dcpLSRay_res.us == 0.75);
  CHECK(dcpLSRay_res.cps == vec2(5.0, 0.0));
  CHECK(dcpLSRay_res.cpr == vec2(5.0, 0.0));
}

TEST(Stack_DgR2SegmentSegment, DgR2Segment)
{
  seg ls0, ls1;
  ls0.Set(vec2(2.0, 0.0), vec2(6.0, 0.0));

  Dg::CP2SegmentSegment<Real>            dcpLSLS;
  Dg::CP2SegmentSegment<Real>::Result    dcpLSLS_res;

  //LineSegs parallel, no overlap, closest points ls0-p0, ls1-p0
  ls1.Set(vec2(-1.0, -4.0), vec2(-5.0, -4.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());

  //LineSegs parallel, no overlap, closest points ls0-p0, ls1-p1
  ls1.Set(vec2(-5.0, -4.0), vec2(-1.0, -4.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 1.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP1());

  //LineSegs parallel, no overlap, closest points ls0-p1, ls1-p0
  ls1.Set(vec2(9.0, -4.0), vec2(18.0, -4.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());

  //LineSegs parallel, no overlap, closest points ls0-p1, ls1-p1
  ls1.Set(vec2(10.0, -4.0), vec2(9.0, -4.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 1.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP1());

  //LineSegs parallel, overlap, ls0p0---ls1p0---ls0p1---ls1p1
  ls1.Set(vec2(4.0, -3.0), vec2(10.0, -3.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSLS_res.u0 == 0.5);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == vec2(4.0, 0.0));
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());

  //LineSegs parallel, overlap, ls0p1---ls1p0---ls0p0---ls1p1
  ls1.Set(vec2(4.0, -3.0), vec2(0.0, -3.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 0.5);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == vec2(2.0, -3.0));

  //LineSegs parallel, overlap, ls0p0---ls1p0---ls1p1---ls0p1
  ls1.Set(vec2(4.0, -3.0), vec2(5.0, -3.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSLS_res.u0 == 0.5);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == vec2(4.0, 0.0));
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());

  //LineSegs parallel, overlap, ls0p1---ls1p0---ls1p1---ls0p0
  ls1.Set(vec2(4.0, -3.0), vec2(8.0, -3.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Overlapping);
  CHECK(dcpLSLS_res.u0 == 0.5);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == vec2(4.0, 0.0));
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());

  //LineSegs not parallel, closest points: ls0p0, ls1p0
  ls1.Set(vec2(-2.0, -5.0), vec2(-2.0, -6.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());

  //LineSegs not parallel, closest points: ls0p0, ls1p1
  ls1.Set(vec2(-2.0, -5.0), vec2(-2.0, -4.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 1.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP1());

  //LineSegs not parallel, closest points: ls0p1, ls1p0
  ls1.Set(vec2(10.0, -5.0), vec2(10.0, -7.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());

  //LineSegs not parallel, closest points: ls0p1, ls1p1
  ls1.Set(vec2(10.0, -5.0), vec2(10.0, -1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 1.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP1());

  //LineSegs not parallel, closest points: ls0p0, ls1-along ls
  ls1.Set(vec2(-1.0, -4.0), vec2(-1.0, 4.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 0.5);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == vec2(-1.0, 0.0));

  //LineSegs not parallel, closest points: ls0p1, ls1-along ls
  ls1.Set(vec2(9.0, 4.0), vec2(9.0, -4.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 0.5);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == vec2(9.0, 0.0));

  //LineSegs intersect
  ls1.Set(vec2(4.0, 4.0), vec2(4.0, -4.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == Dg::QueryCode::Success);
  CHECK(dcpLSLS_res.u0 == 0.5);
  CHECK(dcpLSLS_res.u1 == 0.5);
  CHECK(dcpLSLS_res.cp0 == vec2(4.0, 0.0));
  CHECK(dcpLSLS_res.cp1 == vec2(4.0, 0.0));
}