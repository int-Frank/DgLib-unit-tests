#include "TestHarness.h"
#include "DgZAlignedCylinder.h"
#include "DgQueryPointZAlignedCylinder.h"

typedef Dg::Vector3<float>      vec3;
typedef Dg::ZAlignedCylinder<float>   zCyl;

TEST(Stack_DgZCyl0, DgDgZCyl0)
{
  zCyl zc(vec3(1.0f, 2.0f, 3.0f), 2.0f, 23.0f);

  CHECK(zc.GetHeight() == 23.0f);
  CHECK(zc.GetRadius() == 2.0f);
  CHECK(zc.GetBottomCenter() == vec3(1.0f, 2.0f, 3.0f));

  zc.SetRadius(1.0f);
  zc.SetHeight(2.0f);
  zc.SetBottom(vec3(1.0f, 1.0f, 1.0f));

  CHECK(zc.GetHeight() == 2.0f);
  CHECK(zc.GetRadius() == 1.0f);
  CHECK(zc.GetBottomCenter() == vec3(1.0f, 1.0f, 1.0f));

  vec3 p = Dg::Zeros<float, 3>();

  Dg::CP3PointZCylinder<float> query;
  Dg::CP3PointZCylinder<float>::Result result = query(p, zc);
  
  float cos45 = 0.70710678118654752440084436210485f;  //or 1/sqrt(2)
  float sqrt2 = 1.4142135623730950488016887242097f;

  //outside infinite cylinder, below
  float val = (sqrt2 - 1.0f)*cos45;
  CHECK(result.cp == vec3(val, val, 1.0f));
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  //outside infinite cylinder, above
  p = vec3(0.0f, 0.0f, 10.0f);
  result = query(p, zc);
  CHECK(result.cp == vec3(val, val, 3.0f));
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  //outside infinite cylinder, inner
  p = vec3(3.0f, 3.0f, 2.0f);
  val = cos45 + 1.0f;
  result = query(p, zc);
  CHECK(result.cp == vec3(val, val, 2.0f));
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  //inside infinite cylinder, above
  p = vec3(1.0f, 1.0f, 12.0f);
  result = query(p, zc);
  CHECK(result.cp == vec3(1.0f, 1.0f, 3.0f));
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  //inside infinite cylinder, below
  p = vec3(1.0f, 1.0f, -12.0f);
  result = query(p, zc);
  CHECK(result.cp == vec3(1.0f, 1.0f, 1.0f));
  CHECK(result.code == Dg::QueryCode::NotIntersecting);

  //On cylinder top boundary
  p = vec3(1.0f, 1.0f, 3.0f);
  result = query(p, zc);
  CHECK(result.cp == p);
  CHECK(result.code == Dg::QueryCode::Intersecting);

  //Inside
  p = vec3(1.0f, 1.0f, 1.5f);
  result = query(p, zc);
  CHECK(result.cp == p);
  CHECK(result.code == Dg::QueryCode::Intersecting);

  //On cylinder bottom boundary
  p = vec3(1.0f, 1.0f, 1.0f);
  result = query(p, zc);
  CHECK(result.cp == p);
  CHECK(result.code == Dg::QueryCode::Intersecting);
}
