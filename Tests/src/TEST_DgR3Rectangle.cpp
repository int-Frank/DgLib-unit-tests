#include "TestHarness.h"
#include "DgRectangle.h"
#include "DgVector.h"
#include "DgQueryRectanglePoint.h"
#include "DgQueryRectangleSphere.h"
#include "DgQueryRectangleLine.h"
#include "DgQueryRectangleRay.h"
#include "DgQueryRectangleSegment.h"

typedef Dg::Rectangle3<float>  Rect;
typedef Dg::Vector3<float>     vec3;
typedef Dg::Sphere<float>     Sphere;
typedef Dg::Line3<float>       Line;
typedef Dg::Ray3<float>        Ray;
typedef Dg::Segment3<float>    Segment;

TEST(Stack_DgRectangle, DgRectangle)
{
  Rect r;
  CHECK(r.GetCenter() == (Dg::Zeros<float, 3>()));
  CHECK(r.GetAxis(0) == (Dg::XAxis<float, 3>()));
  CHECK(r.GetAxis(1) == (Dg::YAxis<float, 3>()));
  CHECK(r.GetExtent(0) == 1.0f);
  CHECK(r.GetExtent(1) == 1.0f);

  r.SetCenter(vec3(1.0f, 2.0f, 3.0f));
  r.SetExtent(0, 2.0f);
  r.SetExtent(1, 3.0f);
  r.SetAxis(0, Dg::ZAxis<float, 3>());
  r.SetAxis(1, -Dg::XAxis<float, 3>());

  CHECK(r.GetCenter() == vec3(1.0f, 2.0f, 3.0f));
  CHECK(r.GetAxis(0) == (Dg::ZAxis<float, 3>()));
  CHECK(r.GetAxis(1) == (-Dg::XAxis<float, 3>()));
  CHECK(r.GetExtent(0) == 2.0f);
  CHECK(r.GetExtent(1) == 3.0f);

  vec3 verts[4];
  r.GetVertices(verts[0], verts[1], verts[2], verts[3]);
  CHECK(verts[0] == vec3(-2.0f, 2.0f, 1.0f));
  CHECK(verts[1] == vec3(4.0f, 2.0f, 1.0f));
  CHECK(verts[2] == vec3(-2.0f, 2.0f, 5.0f));
  CHECK(verts[3] == vec3(4.0f, 2.0f, 5.0f));

  vec3 p(0.0f, 0.0f, 0.0f);
  Dg::CP3RectanglePoint<float>  query;
  Dg::CP3RectanglePoint<float>::Result result = query(r, p);
  vec3 cp(0.0f, 2.0f, 1.0f);
  vec3 v = cp - p;
  CHECK(result.cp == cp);
  CHECK(result.sqDistance == Dg::MagSq(v));
  CHECK(result.rectangleParameter[0] == -2.0f);
  CHECK(result.rectangleParameter[1] == 1.0f);

  p = vec3(0.0f, 0.0f, 3.0f);
  result = query(r, p);
  cp = vec3(0.0f, 2.0f, 3.0f);
  v = cp - p;
  CHECK(result.cp == cp);
  CHECK(result.sqDistance == Dg::MagSq(v));
  CHECK(result.rectangleParameter[0] == 0.0f);
  CHECK(result.rectangleParameter[1] == 1.0f);

  p = vec3(10.0f, 0.0f, 30.0f);
  result = query(r, p);
  cp = vec3(4.0f, 2.0f, 5.0f);
  v = cp - p;
  CHECK(result.cp == cp);
  CHECK(result.sqDistance == Dg::MagSq(v));
  CHECK(result.rectangleParameter[0] == 2.0f);
  CHECK(result.rectangleParameter[1] == -3.0f);

  p = vec3(-10.0f, 10.0f, 30.0f);
  result = query(r, p);
  cp = vec3(-2.0f, 2.0f, 5.0f);
  v = cp - p;
  CHECK(result.cp == cp);
  CHECK(result.sqDistance == Dg::MagSq(v));
  CHECK(result.rectangleParameter[0] == 2.0f);
  CHECK(result.rectangleParameter[1] == 3.0f);

  p = vec3(-10.0f, 0.0f, 0.0f);
  result = query(r, p);
  cp = vec3(-2.0f, 2.0f, 1.0f);
  v = cp - p;
  CHECK(result.cp == cp);
  CHECK(result.sqDistance == Dg::MagSq(v));
  CHECK(result.rectangleParameter[0] == -2.0f);
  CHECK(result.rectangleParameter[1] == 3.0f);
}

TEST(Stack_DgRectangle_querySphere, DgRectangle_querySphere)
{
  Rect r;
  r.SetCenter(vec3(1.0f, 2.0f, 3.0f));
  r.SetExtent(0, 2.0f);
  r.SetExtent(1, 3.0f);
  r.SetAxis(0, Dg::ZAxis<float, 3>());
  r.SetAxis(1, -Dg::XAxis<float, 3>());

  Sphere s;
  s.SetCenter(Dg::Zeros<float, 3>());
  s.SetRadius(1.0f);

  Dg::TI3RectangleSphere<float> query;
  Dg::TI3RectangleSphere<float>::Result result = query(r, s);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  s.SetRadius(4.0f);
  result = query(r, s);
  CHECK(result.code == Dg::QueryCode::Intersecting);

  s.SetRadius(2.0f);
  s.SetCenter(vec3(0.0f, 0.0f, 3.0f));
  result = query(r, s);
  CHECK(result.code == Dg::QueryCode::Intersecting);

  s.SetRadius(1.9f);
  s.SetCenter(vec3(0.0f, 0.0f, 3.0f));
  result = query(r, s);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);
}

TEST(Stack_DgRectangle_queryLine, DgRectangle_queryLine)
{
  Rect r;
  r.SetCenter(vec3(1.0f, 2.0f, 3.0f));
  r.SetExtent(0, 2.0f);
  r.SetExtent(1, 3.0f);
  r.SetAxis(0, Dg::ZAxis<float, 3>());
  r.SetAxis(1, -Dg::XAxis<float, 3>());

  Dg::FI3RectangleLine<float> query;
  Dg::FI3RectangleLine<float>::Result result;
  Line l;

  //No intersections...
  l.Set(vec3(-3.0f, 0.0f, 0.0f), Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(0.0f, 0.0f, 0.0f), Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(5.0f, 0.0f, 0.0f), Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(-5.0f, 0.0f, 3.0f), Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(5.0f, 0.0f, 3.0f), Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(-5.0f, 0.0f, 33.0f), Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(1.0f, 0.0f, 33.0f), Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(10.0f, 0.0f, 33.0f), Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  //Now the same but from the opposite side
  l.Set(vec3(-3.0f, 10.0f, 0.0f), -Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(0.0f, 20.0f, 0.0f), -Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(5.0f, 30.0f, 0.0f), -Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(-5.0f, 10.0f, 3.0f), -Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(5.0f, 20.0f, 3.0f), -Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(-5.0f, 30.0f, 33.0f), -Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(1.0f, 10.0f, 33.0f), -Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(10.0f, 20.0f, 33.0f), -Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  //Intersecting...

  //Absolutely intersecting..
  l.Set(vec3(-1.0f, 0.0f, 2.0f), Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(-1.0f, 2.0f, 2.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == 2.0f);

  l.Set(vec3(-1.0f, 0.0f, 4.0f), Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(-1.0f, 2.0f, 4.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 1.0f);
  CHECK(result.rectangleParameter[1] == 2.0f);

  l.Set(vec3(3.0f, 0.0f, 4.0f), Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(3.0f, 2.0f, 4.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 1.0f);
  CHECK(result.rectangleParameter[1] == -2.0f);

  l.Set(vec3(3.0f, 0.0f, 2.0f), Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(3.0f, 2.0f, 2.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == -2.0f);

  //From the other side...
  l.Set(vec3(-1.0f, 4.0f, 2.0f), -Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(-1.0f, 2.0f, 2.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == 2.0f);

  l.Set(vec3(-1.0f, 4.0f, 4.0f), -Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(-1.0f, 2.0f, 4.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 1.0f);
  CHECK(result.rectangleParameter[1] == 2.0f);

  l.Set(vec3(3.0f, 4.0f, 4.0f), -Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(3.0f, 2.0f, 4.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 1.0f);
  CHECK(result.rectangleParameter[1] == -2.0f);

  l.Set(vec3(3.0f, 4.0f, 2.0f), -Dg::YAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(3.0f, 2.0f, 2.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == -2.0f);

  //Parallel, not intersecting...

  l.Set(vec3(1.0f, 1.5f, 1.0f), Dg::XAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  l.Set(vec3(1.0f, 2.0f, 0.5f), Dg::XAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  //Parallel, intersecting
  //line direction is xAxis
  l.Set(vec3(1.0f, 2.0f, 2.0f), Dg::XAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(-2.0f, 2.0f, 2.0f));
  CHECK(result.u == -3.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == 3.0f);

  l.Set(vec3(-3.0f, 2.0f, 2.0f), Dg::XAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(-2.0f, 2.0f, 2.0f));
  CHECK(result.u == 1.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == 3.0f);

  l.Set(vec3(5.0f, 2.0f, 2.0f), -Dg::XAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(4.0f, 2.0f, 2.0f));
  CHECK(result.u == 1.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == -3.0f);

  //line direction is zAxis
  l.Set(vec3(1.0f, 2.0f, -1.0f), Dg::ZAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(1.0f, 2.0f, 1.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -2.0f);
  CHECK(result.rectangleParameter[1] == 0.0f);

  l.Set(vec3(1.0f, 2.0f, 7.0f), -Dg::ZAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(1.0f, 2.0f, 5.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 2.0f);
  CHECK(result.rectangleParameter[1] == 0.0f);

  //On rectangle edge
  l.Set(vec3(-3.0f, 2.0f, 1.0f), Dg::XAxis<float, 3>());
  result = query(r, l);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(-2.0f, 2.0f, 1.0f));
  CHECK(result.u == 1.0f);
  CHECK(result.rectangleParameter[0] == -2.0f);
  CHECK(result.rectangleParameter[1] == 3.0f);
}

TEST(Stack_DgRectangle_queryRay, DgRectangle_queryRay)
{
  Rect r;
  r.SetCenter(vec3(1.0f, 2.0f, 3.0f));
  r.SetExtent(0, 2.0f);
  r.SetExtent(1, 3.0f);
  r.SetAxis(0, Dg::ZAxis<float, 3>());
  r.SetAxis(1, -Dg::XAxis<float, 3>());

  Dg::FI3RectangleRay<float> query;
  Dg::FI3RectangleRay<float>::Result result;
  Ray ray;

  //No intersections...
  ray.Set(vec3(-3.0f, 0.0f, 0.0f), Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(0.0f, 0.0f, 0.0f), Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(5.0f, 0.0f, 0.0f), Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(-5.0f, 0.0f, 3.0f), Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(5.0f, 0.0f, 3.0f), Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(-5.0f, 0.0f, 33.0f), Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(1.0f, 0.0f, 33.0f), Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(10.0f, 0.0f, 33.0f), Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  //Now the same but from the opposite side
  ray.Set(vec3(-3.0f, 10.0f, 0.0f), -Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(0.0f, 20.0f, 0.0f), -Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(5.0f, 30.0f, 0.0f), -Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(-5.0f, 10.0f, 3.0f), -Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(5.0f, 20.0f, 3.0f), -Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(-5.0f, 30.0f, 33.0f), -Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(1.0f, 10.0f, 33.0f), -Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(10.0f, 20.0f, 33.0f), -Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  //Intersecting...

  //Absolutely intersecting..
  ray.Set(vec3(-1.0f, 0.0f, 2.0f), Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(-1.0f, 2.0f, 2.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == 2.0f);

  ray.Set(vec3(-1.0f, 0.0f, 4.0f), Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(-1.0f, 2.0f, 4.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 1.0f);
  CHECK(result.rectangleParameter[1] == 2.0f);

  ray.Set(vec3(3.0f, 0.0f, 4.0f), Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(3.0f, 2.0f, 4.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 1.0f);
  CHECK(result.rectangleParameter[1] == -2.0f);

  ray.Set(vec3(3.0f, 0.0f, 2.0f), Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(3.0f, 2.0f, 2.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == -2.0f);

  //From the other side...
  ray.Set(vec3(-1.0f, 4.0f, 2.0f), -Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(-1.0f, 2.0f, 2.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == 2.0f);

  ray.Set(vec3(-1.0f, 4.0f, 4.0f), -Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(-1.0f, 2.0f, 4.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 1.0f);
  CHECK(result.rectangleParameter[1] == 2.0f);

  ray.Set(vec3(3.0f, 4.0f, 4.0f), -Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(3.0f, 2.0f, 4.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 1.0f);
  CHECK(result.rectangleParameter[1] == -2.0f);

  ray.Set(vec3(3.0f, 4.0f, 2.0f), -Dg::YAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(3.0f, 2.0f, 2.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == -2.0f);

  //Parallel, not intersecting...

  ray.Set(vec3(1.0f, 1.5f, 1.0f), Dg::XAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  ray.Set(vec3(1.0f, 2.0f, 0.5f), Dg::XAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  //Parallel, intersecting
  //line direction is xAxis
  ray.Set(vec3(1.0f, 2.0f, 2.0f), Dg::XAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(4.0f, 2.0f, 2.0f));
  CHECK(result.u == 3.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == -3.0f);

  ray.Set(vec3(-3.0f, 2.0f, 2.0f), Dg::XAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(-2.0f, 2.0f, 2.0f));
  CHECK(result.u == 1.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == 3.0f);

  ray.Set(vec3(5.0f, 2.0f, 2.0f), -Dg::XAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(4.0f, 2.0f, 2.0f));
  CHECK(result.u == 1.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == -3.0f);

  //line direction is zAxis
  ray.Set(vec3(1.0f, 2.0f, -1.0f), Dg::ZAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(1.0f, 2.0f, 1.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -2.0f);
  CHECK(result.rectangleParameter[1] == 0.0f);

  ray.Set(vec3(1.0f, 2.0f, 7.0f), -Dg::ZAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(1.0f, 2.0f, 5.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 2.0f);
  CHECK(result.rectangleParameter[1] == 0.0f);

  //On rectangle edge
  ray.Set(vec3(-3.0f, 2.0f, 1.0f), Dg::XAxis<float, 3>());
  result = query(r, ray);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(-2.0f, 2.0f, 1.0f));
  CHECK(result.u == 1.0f);
  CHECK(result.rectangleParameter[0] == -2.0f);
  CHECK(result.rectangleParameter[1] == 3.0f);
}

TEST(Stack_DgRectangle_querySegment, DgRectangle_querySegment)
{
  Rect r;
  r.SetCenter(vec3(1.0f, 2.0f, 3.0f));
  r.SetExtent(0, 2.0f);
  r.SetExtent(1, 3.0f);
  r.SetAxis(0, Dg::ZAxis<float, 3>());
  r.SetAxis(1, -Dg::XAxis<float, 3>());

  Dg::FI3RectangleSegment<float> query;
  Dg::FI3RectangleSegment<float>::Result result;
  Segment seg;

  //No intersections...
  seg.Set(vec3(-3.0f, 0.0f, 0.0f), vec3(-3.0f, 4.0f, 0.0f));
  result = query(r, seg);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  seg.Set(vec3(1.0f, 0.0f, 3.0f), vec3(1.0f, 1.0f, 3.0f));
  result = query(r, seg);
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  //Intersections
  seg.Set(vec3(1.0f, 0.0f, 3.0f), vec3(1.0f, 4.0f, 3.0f));
  result = query(r, seg);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(1.0f, 2.0f, 3.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 0.0f);
  CHECK(result.rectangleParameter[1] == 0.0f);

  seg.Set(vec3(1.0f, 4.0f, 3.0f), vec3(1.0f, -3.0f, 3.0f));
  result = query(r, seg);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(1.0f, 2.0f, 3.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 0.0f);
  CHECK(result.rectangleParameter[1] == 0.0f);

  seg.Set(vec3(0.0f, 4.0f, 2.0f), vec3(0.0f, -3.0f, 2.0f));
  result = query(r, seg);
  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.point == vec3(0.0f, 2.0f, 2.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == 1.0f);
}