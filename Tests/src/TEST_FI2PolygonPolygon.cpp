
#include <cmath>
#include <iostream>

#include "TestHarness.h"
#include "DgPolygon.h"
#include "DgQueryPolygonPolygon.h"

typedef float                       Real;
typedef Dg::Vector2<Real>           vec;
typedef Dg::Segment2<Real>          seg;
typedef Dg::Polygon2<Real>          polygon;
typedef Dg::FI2PolygonPolygon<Real> fiquery;

typedef Dg::impl_FI2PolygonPolygon::Graph<Real> graph;
typedef Dg::impl_FI2PolygonPolygon::Node<Real> node;
typedef Dg::impl_FI2PolygonPolygon::GraphBuilder<Real> graphBuilder;

void PrintGraph(graph const &graph)
{
  std::cout << "\n\nGRAPH";

  for (size_t i = 0; i < graph.nodes.size(); i++)
  {
    node const *pNode = &(graph.nodes[i]);
    std::cout << '\n' << i << ": (" << pNode->vertex.x() << ", " << pNode->vertex.y() << "); [";
    for (size_t j = 0; j < pNode->neighbours.size(); j++)
    {
      std::cout << pNode->neighbours[j].id;
      if (j + 1 != pNode->neighbours.size())
        std::cout << ", ";
    }
    std::cout << "]";
  }
  std::cout << "\n\n";
}

TEST(Stack_FIPolygonPolygon, FIPP_Orientation)
{
  bool good = true;
  int const nSegs = 1000;
  vec a(1.f, 0.f);
  float prev = 42.f;

  for (int i = 0; i < nSegs; i++)
  {
    float angle = Dg::Constants<float>::PI * 2.f * (float)i / (float)nSegs;
    float x = std::cos(angle);
    float y = std::sin(angle);
    vec b(x, y);
    float val = Dg::impl_FI2PolygonPolygon::GetOrientation(b, a);
    if (val > prev)
    {
      good = false;
      break;
    }
    prev = val;
  }

  CHECK(good);
}

TEST(Stack_FIPolygonPolygon, FIPP_NotIntersecting)
{
  polygon polyA;
  polygon polyB;

  polyA.PushBack(vec(0.f, 0.f));
  polyA.PushBack(vec(1.f, 0.f));
  polyA.PushBack(vec(1.f, 1.f));
  polyA.PushBack(vec(0.f, 1.f));

  polyB.PushBack(vec(10.f, 0.f));
  polyB.PushBack(vec(11.f, 0.f));
  polyB.PushBack(vec(11.f, 1.f));
  polyB.PushBack(vec(10.f, 1.f));

  fiquery query;
  auto result = query(polyA, polyB);

  CHECK(result.code == Dg::QueryCode::NotIntersecting);
}

TEST(Stack_FIPolygonPolygon, FIPP_Graph_OverlappingPoint_0)
{
  polygon polyA;
  polygon polyB;

  polyA.PushBack(vec(0.f, 0.f));
  polyA.PushBack(vec(1.f, 0.f));
  polyA.PushBack(vec(1.f, 1.f));
  polyA.PushBack(vec(0.f, 1.f));

  polyB.PushBack(vec(1.f, 1.f));
  polyB.PushBack(vec(2.f, 1.f));
  polyB.PushBack(vec(2.f, 2.f));
  polyB.PushBack(vec(1.f, 2.f));

  graph graph;
  graphBuilder builder;
  auto code = builder.Execute(polyA, polyB, &graph);

  CHECK(code == Dg::QueryCode::Intersecting);
}

TEST(Stack_FIPolygonPolygon, FIPP_Graph_OverlappingPoint_1)
{
  polygon polyA;
  polygon polyB;

  polyA.PushBack(vec(0.f, 0.f));
  polyA.PushBack(vec(1.f, 0.f));
  polyA.PushBack(vec(1.f, 1.f));
  polyA.PushBack(vec(0.f, 1.f));

  polyB.PushBack(vec(1.f, 1.f));
  polyB.PushBack(vec(1.f, 2.f));
  polyB.PushBack(vec(0.f, 2.f));
  polyB.PushBack(vec(0.f, 1.f));
  polyB.PushBack(vec(1.5f, 1.5f));

  graph graph;
  graphBuilder builder;
  auto code = builder.Execute(polyA, polyB, &graph);

  CHECK(code == Dg::QueryCode::Intersecting);
}

TEST(Stack_FIPolygonPolygon, FIPP_Graph_OverlappingEdge_0)
{
  polygon polyA;
  polygon polyB;

  polyA.PushBack(vec(0.f, 0.f));
  polyA.PushBack(vec(1.f, 0.f));
  polyA.PushBack(vec(1.f, 1.f));
  polyA.PushBack(vec(0.f, 1.f));

  polyB.PushBack(vec(1.f, 1.f));
  polyB.PushBack(vec(1.f, 2.f));
  polyB.PushBack(vec(0.f, 2.f));
  polyB.PushBack(vec(0.f, 1.f));

  graph graph;
  graphBuilder builder;
  auto code = builder.Execute(polyA, polyB, &graph);

  CHECK(code == Dg::QueryCode::Intersecting);
}