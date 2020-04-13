#include "TestHarness.h"
#include "DgR2AABB.h"
#include "DgR2Vector.h"
#include "DgR2QueryPointAABB.h"

typedef Dg::R2::Vector<float>     vec;
typedef Dg::R2::AABB<float>       AABB;

TEST(Stack_DgR2AABB, DgR2AABB)
{
  AABB aabb(vec(-1.0f, -1.0f, 1.0f), vec(1.0f, 1.0f, 1.0f));

  Dg::R2::TIPointAABB<float> query;
  CHECK(query(vec(0.0f, 0.0f, 1.0f), aabb).isIntersecting == true);
  CHECK(query(vec(0.9f, 0.9f, 1.0f), aabb).isIntersecting == true);
  CHECK(query(vec(-0.9f, -0.9f, 1.0f), aabb).isIntersecting == true);

  CHECK(query(vec(1.1f, 0.0f, 1.0f), aabb).isIntersecting  == false);
  CHECK(query(vec(0.0f, 1.1f, 1.0f), aabb).isIntersecting  == false);
  CHECK(query(vec(-2.0f, 0.0f, 1.0f), aabb).isIntersecting == false);
  CHECK(query(vec(0.0f, -2.0f, 1.0f), aabb).isIntersecting == false);
}