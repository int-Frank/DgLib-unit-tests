#include "TestHarness.h"
#include "DgAABB.h"
#include "DgVector.h"
#include "DgQueryPointAABB.h"

typedef Dg::Vector2<float>     vec2;
typedef Dg::AABB2<float>       AABB2;

TEST(Stack_DgR2AABB, DgR2AABB)
{
  AABB2 aabb(vec2(-1.0f, -1.0f), vec2(1.0f, 1.0f));

  Dg::TI2PointAABB<float> query;
  CHECK(query(vec2(0.0f, 0.0f), aabb).isIntersecting == true);
  CHECK(query(vec2(0.9f, 0.9f), aabb).isIntersecting == true);
  CHECK(query(vec2(-0.9f, -0.9f), aabb).isIntersecting == true);

  CHECK(query(vec2(1.1f, 0.0f), aabb).isIntersecting  == false);
  CHECK(query(vec2(0.0f, 1.1f), aabb).isIntersecting  == false);
  CHECK(query(vec2(-2.0f, 0.0f), aabb).isIntersecting == false);
  CHECK(query(vec2(0.0f, -2.0f), aabb).isIntersecting == false);
}