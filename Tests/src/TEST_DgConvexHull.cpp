#include "TestHarness.h"
#include "DgR2ConvexHull.h"
#include "DgDynamicArray.h"

TEST(Stack_DgR2ConvexHull, creation_DgR2ConvexHull)
{
  typedef Dg::R2::Vector<float> vec;
  typedef Dg::DynamicArray<vec> Container;

  vec points[] ={vec(0.0f, 3.0f, 1.0f),
                 vec(1.0f, 1.0f, 1.0f),
                 vec(2.0f, 2.0f, 1.0f),
                 vec(4.0f, 4.0f, 1.0f),
                 vec(0.0f, 0.0f, 1.0f),
                 vec(1.0f, 2.0f, 1.0f),
                 vec(3.0f, 1.0f, 1.0f),
                 vec(3.0f, 3.0f, 1.0f)};
  size_t nItems = sizeof(points) / sizeof(points[0]);
  Container result;
  Dg::R2::ConvexHull<float, Container>(points, nItems, result);

  CHECK(result.size() == 4);

  CHECK(result[0] == vec(0.0f, 0.0f, 1.0f));
  CHECK(result[1] == vec(3.0f, 1.0f, 1.0f));
  CHECK(result[2] == vec(4.0f, 4.0f, 1.0f));
  CHECK(result[3] == vec(0.0f, 3.0f, 1.0f));
}

TEST(Stack_DgR2ConvexHull_Mutable, creation_DgR2ConvexHull_Mutable)
{
  typedef Dg::R2::Vector<float> vec;
  typedef Dg::DynamicArray<vec> Container;

  vec points[] ={vec(0.0f, 3.0f, 1.0f),
                 vec(1.0f, 1.0f, 1.0f),
                 vec(2.0f, 2.0f, 1.0f),
                 vec(4.0f, 4.0f, 1.0f),
                 vec(0.0f, 0.0f, 1.0f),
                 vec(1.0f, 2.0f, 1.0f),
                 vec(3.0f, 1.0f, 1.0f),
                 vec(3.0f, 3.0f, 1.0f)};
  size_t nItems = sizeof(points) / sizeof(points[0]);
  Container result;
  Dg::R2::ConvexHull_Mutable<float, Container>(points, nItems, result);

  CHECK(result.size() == 4);

  CHECK(result[0] == vec(0.0f, 0.0f, 1.0f));
  CHECK(result[1] == vec(3.0f, 1.0f, 1.0f));
  CHECK(result[2] == vec(4.0f, 4.0f, 1.0f));
  CHECK(result[3] == vec(0.0f, 3.0f, 1.0f));
}