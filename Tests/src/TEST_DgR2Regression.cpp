#include "TestHarness.h"
#include "DgRegression.h"

typedef Dg::Vector2<float> vec2;
typedef Dg::Line2<float> line;

TEST(Stack_DgR2Regression, DgR2Regression)
{
  vec2 points[4] = { vec2(1.f, 3.f), 
                     vec2(2.f, 5.f), 
                     vec2(3.f, 5.f),
                     vec2(1.5f, 1.7f) };


  line l = Dg::LineOfBestFit2(points, 4);
}