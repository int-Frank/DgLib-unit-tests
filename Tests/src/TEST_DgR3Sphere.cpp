#include "TestHarness.h"
#include "DgHypersphere.h"
#include "DgLine.h"
#include "DgQueryLineHypersphere.h"

typedef double                  Real;
typedef Dg::Vector3<Real>       vec3;
typedef Dg::Line3<Real>          line;
typedef Dg::Sphere<Real>        sphere;

TEST(Stack_DgSphere, DgSphere)
{
  vec3 center(2.0, 6.0, 9.0);
  Real radius = 4.0;

  sphere s = sphere(center, radius);
  CHECK(s.Center() == center);
  CHECK(s.Radius() == radius);

  sphere s0 = s;
  sphere s1(s0);

  s.SetCenter(center);
  s.SetRadius(radius);
  CHECK(s.Center() == center);
  CHECK(s.Radius() == radius);

  s.Set(center, radius);
  CHECK(s.Center() == center);
  CHECK(s.Radius() == radius);
}