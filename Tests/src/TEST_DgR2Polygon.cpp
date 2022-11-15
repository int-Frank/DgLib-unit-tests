#include "TestHarness.h"
#include "DgPolygon.h"
#include "DgQueryPointPolygon.h"
#include "DgQueryPolygonPolygon.h"

typedef float Real;
typedef Dg::Vector2<Real>  vec;
typedef Dg::Segment2<Real>  seg;
typedef Dg::Polygon2<Real> polygon;

static bool CheckEqual(vec const &a, vec const &b)
{
  return (a.x() == b.x()) && (a.y() == b.y());
}

static bool CheckEqual(seg const &a, seg const &b)
{
  return CheckEqual(a.GetP0(), b.GetP0()) && CheckEqual(a.GetP1(), b.GetP1());
}

static bool CheckEqual(polygon const &a, polygon const &b)
{
  if (a.Size() != b.Size())
    return false;

  auto pa_it = a.cPointsBegin();
  auto pb_it = b.cPointsBegin();

  auto ea_it = a.cEdgesBegin();
  auto eb_it = b.cEdgesBegin();

  for (size_t i = 0; i < a.Size(); i++, pa_it++, pb_it++, ea_it++, eb_it++)
  {
    if (!CheckEqual(*pa_it, *pb_it))
      return false;

    if (!CheckEqual(*ea_it, *eb_it))
      return false;
  }

  eb_it--;
  ea_it--;

  if (!CheckEqual(*ea_it, *eb_it))
    return false;

  return true;
}

TEST(Stack_DgR2Polygon, construction_DgR2Polygon)
{
  polygon poly_a;

  poly_a.PushBack(vec(1.0f, 1.1f));
  poly_a.PushBack(vec(1.2f, -1.3f));
  poly_a.PushBack(vec(-1.4f, -1.5f));
  poly_a.PushBack(vec(-1.6f, 1.7f));

  polygon poly_b = poly_a;
  polygon poly_c(poly_a);

  CHECK(CheckEqual(poly_a, poly_b));
  CHECK(CheckEqual(poly_a, poly_c));

  poly_c = poly_b;
  CHECK(CheckEqual(poly_b, poly_c));
}

TEST(Stack_DgR2Polygon, creation_DgR2Polygon)
{
  polygon poly;

  CHECK(poly.Winding() == Dg::Orientation::Colinear);

  poly.PushBack(vec(1.f, 1.f));
  CHECK(poly.Winding() == Dg::Orientation::Colinear);

  poly.PushBack(vec(1.f, -1.f));
  CHECK(poly.Winding() == Dg::Orientation::Colinear);

  poly.PushBack(vec(-1.f, -1.f));
  CHECK(poly.Winding() == Dg::Orientation::CW);

  poly.PushBack(vec(-1.f, 1.f));
  CHECK(poly.Winding() == Dg::Orientation::CW);

  CHECK(poly.Area() == 4.f);

  poly.SetWinding(Dg::Orientation::CCW);
  CHECK(poly.Winding() == Dg::Orientation::CCW);

  poly.Clear();
  CHECK(poly.Winding() == Dg::Orientation::Colinear);

  poly.PushBack(vec(1.f, 1.f));
  CHECK(poly.Winding() == Dg::Orientation::Colinear);

  poly.PushBack(vec(-1.f, 1.f));
  CHECK(poly.Winding() == Dg::Orientation::Colinear);

  poly.PushBack(vec(-1.f, -1.f));
  CHECK(poly.Winding() == Dg::Orientation::CCW);

  poly.PushBack(vec(1.f, -1.f));
  CHECK(poly.Winding() == Dg::Orientation::CCW);

  CHECK(poly.Area() == 4.f);

  poly.SetWinding(Dg::Orientation::CW);
  CHECK(poly.Winding() == Dg::Orientation::CW);

}

TEST(Stack_DgR2Polygon, query_DgR2PointPolygon)
{
  polygon poly;
  vec p;
  Dg::TI2PointPolygon<Real> query;
  Dg::TI2PointPolygon<Real>::Result result;

  poly.PushBack(vec(1.f, 1.f));
  poly.PushBack(vec(1.f, -1.f));
  poly.PushBack(vec(-1.f, -1.f));
  poly.PushBack(vec(-1.f, 1.f));

  // Inside

  p = vec(0.f, 0.f);
  result = query(p, poly);
  CHECK(result.code == Dg::QueryCode::CompletelyInside);

  // On boundary

  p = vec(1.f, 0.f);
  result = query(p, poly);
  CHECK(result.code == Dg::QueryCode::OnBoundary);

  p = vec(-1.f, 0.f);
  result = query(p, poly);
  CHECK(result.code == Dg::QueryCode::OnBoundary);

  p = vec(0.f, 1.f);
  result = query(p, poly);
  CHECK(result.code == Dg::QueryCode::OnBoundary);

  p = vec(0.f, -1.f);
  result = query(p, poly);
  CHECK(result.code == Dg::QueryCode::OnBoundary);

  // Outside

  p = vec(0.f, 2.f);
  result = query(p, poly);
  CHECK(result.code == Dg::QueryCode::CompletelyOutside);

  p = vec(2.f, 2.f);
  result = query(p, poly);
  CHECK(result.code == Dg::QueryCode::CompletelyOutside);

  p = vec(2.f, 0.f);
  result = query(p, poly);
  CHECK(result.code == Dg::QueryCode::CompletelyOutside);

  p = vec(2.f, -2.f);
  result = query(p, poly);
  CHECK(result.code == Dg::QueryCode::CompletelyOutside);

  p = vec(0.f, -2.f);
  result = query(p, poly);
  CHECK(result.code == Dg::QueryCode::CompletelyOutside);

  p = vec(-2.f, -2.f);
  result = query(p, poly);
  CHECK(result.code == Dg::QueryCode::CompletelyOutside);

  p = vec(-2.f, 0.f);
  result = query(p, poly);
  CHECK(result.code == Dg::QueryCode::CompletelyOutside);

  p = vec(-2.f, 2.f);
  result = query(p, poly);
  CHECK(result.code == Dg::QueryCode::CompletelyOutside);
}

TEST(Stack_DgR2Polygon, query_DgR2PolygonPolygon)
{
  polygon poly_a;
  polygon poly_b;
  vec p;
  Dg::TI2PolygonPolygon<Real> query;
  Dg::TI2PolygonPolygon<Real>::Result result;

  poly_a.PushBack(vec(1.f, 1.f));
  poly_a.PushBack(vec(1.f, -1.f));
  poly_a.PushBack(vec(-1.f, -1.f));
  poly_a.PushBack(vec(-1.f, 1.f));

  poly_b.PushBack(vec(0.5f, 0.5f));
  poly_b.PushBack(vec(0.5f, -0.5f));
  poly_b.PushBack(vec(-0.5f, 0.f));
  result = query(poly_a, poly_b);
  CHECK(result.code == Dg::QueryCode::B_in_A);
  result = query(poly_b, poly_a);
  CHECK(result.code == Dg::QueryCode::A_in_B);

  poly_b.Clear();
  poly_b.PushBack(vec(0.5f, 1.5f));
  poly_b.PushBack(vec(0.5f, 0.5f));
  poly_b.PushBack(vec(-0.5f, 1.f));
  result = query(poly_a, poly_b);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  result = query(poly_b, poly_a);
  CHECK(result.code == Dg::QueryCode::Intersecting);

  poly_b.Clear();
  poly_b.PushBack(vec(0.5f, 2.5f));
  poly_b.PushBack(vec(0.5f, 1.5f));
  poly_b.PushBack(vec(-0.5f, 2.f));
  result = query(poly_a, poly_b);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);
  result = query(poly_b, poly_a);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);
}

vec GetCentroid(Dg::Polygon2<float> const &poly)
{
  float Cx = 0.0f;
  float Cy = 0.0f;
  for (auto it = poly.cEdgesBegin(); it != poly.cEdgesEnd(); it++)
  {
    auto seg = *it;
    vec i = seg.GetP0();
    vec j = seg.GetP1();

    Cx += (i.x() + j.x()) * (i.x() * j.y() - j.x() * i.y());
    Cy += (i.y() + j.y()) * (i.x() * j.y() - j.x() * i.y());
  }

  Cx = Cx / (6.0f * poly.SignedArea());
  Cy = Cy / (6.0f * poly.SignedArea());

  return vec(Cx, Cy);
}

TEST(Stack_DgR2Polygon, dummy)
{
  // Room 
  // Centroid: 0.4633495, 0.4633495
  // Area: 361.667
 
  // Convex Hull
  // Centroid: 0.4641495, 0.4641495
  // Area: 96.87519

  Dg::Polygon2<float> room;
  room.PushBack(vec(-9.379169f, 9.432909f));
  room.PushBack(vec(-9.379169f, -8.939793f));
  room.PushBack(vec(10.30587f, -8.939793f));
  room.PushBack(vec(10.30587f, 9.432909f));

  float roomArea = room.SignedArea();
  vec roomCentroid = GetCentroid(room);

  Dg::Polygon2<float> points;
  points.PushBack(vec(5.384612f, -2.706197f));
  points.PushBack(vec(5.384612f, 7.136323f));
  points.PushBack(vec(-4.457907f, 7.136323f));
  points.PushBack(vec(-4.457908f, -2.706196f));

  float pointsArea = points.SignedArea();
  vec pointsCentroid = GetCentroid(points);

  char t = 0;
}