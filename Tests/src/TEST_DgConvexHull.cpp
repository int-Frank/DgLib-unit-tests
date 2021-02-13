#include "TestHarness.h"
#include "DgConvexHull.h"
#include "DgDynamicArray.h"

typedef Dg::Vector2<float> vec2;
typedef Dg::DynamicArray<vec2> Container;

TEST(Stack_DgR2ConvexHull, creation_DgR2ConvexHull)
{
  vec2 points[] ={vec2(0.0f, 3.0f),
                 vec2(1.0f, 1.0f),
                 vec2(2.0f, 2.0f),
                 vec2(4.0f, 4.0f),
                 vec2(0.0f, 0.0f),
                 vec2(1.0f, 2.0f),
                 vec2(3.0f, 1.0f),
                 vec2(3.0f, 3.0f)};
  size_t nItems = sizeof(points) / sizeof(points[0]);
  Container result;
  Dg::ConvexHull2<float, Container>(points, nItems, result);

  CHECK(result.size() == 4);

  CHECK(result[0] == vec2(0.0f, 0.0f));
  CHECK(result[1] == vec2(3.0f, 1.0f));
  CHECK(result[2] == vec2(4.0f, 4.0f));
  CHECK(result[3] == vec2(0.0f, 3.0f));
}

TEST(Stack_DgR2ConvexHull_Mutable, creation_DgR2ConvexHull_Mutable)
{
  vec2 points[] ={vec2(0.0f, 3.0f),
                 vec2(1.0f, 1.0f),
                 vec2(2.0f, 2.0f),
                 vec2(4.0f, 4.0f),
                 vec2(0.0f, 0.0f),
                 vec2(1.0f, 2.0f),
                 vec2(3.0f, 1.0f),
                 vec2(3.0f, 3.0f)};
  size_t nItems = sizeof(points) / sizeof(points[0]);
  Container result;
  Dg::ConvexHull2_Mutable<float, Container>(points, nItems, result);

  CHECK(result.size() == 4);

  CHECK(result[0] == vec2(0.0f, 0.0f));
  CHECK(result[1] == vec2(3.0f, 1.0f));
  CHECK(result[2] == vec2(4.0f, 4.0f));
  CHECK(result[3] == vec2(0.0f, 3.0f));
}