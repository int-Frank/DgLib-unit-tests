#include "TestHarness.h"
#include "DgHypersphere.h"
#include "DgLine.h"
#include "DgVector.h"
#include "DgQueryLineHypersphere.h"
#include "DgQueryPointHypersphere.h"
#include "DgQueryHypersphereHypersphere.h"

typedef Dg::Vector2<float>      vec2;
typedef Dg::Line2<float>        line;
typedef Dg::Disk2<float>        disk;

TEST(Stack_DgDisk0, DgDisk0)
{
  disk d0(vec2(10.0f, 0.0f), 1.0f);
  disk d1(vec2(-10.0f, 0.0f), 1.0f);

  vec2 v0(-1.0f, 0.0f);
  vec2 v1(1.0f, 0.0f);

  Dg::CPA2DiskDisk<float> fpcQuery;
  Dg::CPA2DiskDisk<float>::Result result = fpcQuery(d0, v0, d1, v1);

  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.t = 9.0f);
}

TEST(Stack_DgDisk2, DgDisk2)
{
  disk d(vec2(10.0f, 0.0f), 1.0f);
  vec2 p(-10.0f, 0.0f);
  vec2 vd(-1.0f, 1.0f);
  vec2 vp(1.0f, 1.0f);

  Dg::CPA2PointDisk<float> fpcQuery;
  Dg::CPA2PointDisk<float>::Result result = fpcQuery(p, vp, d, vd);

  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.t = 9.5f);
}

TEST(Stack_DgDisk3, DgDisk3)
{
  disk d(vec2(10.0f, 0.0f), 1.0f);
  line l(vec2(0.0f, 0.0f), vec2(0.0f, 1.0f));
  vec2 vd(-1.0f, 1.0f);

  Dg::CPA2LineDisk<float> fpcQuery;
  Dg::CPA2LineDisk<float>::Result result = fpcQuery(l, d, vd);

  CHECK(result.code == Dg::QueryCode::Intersecting);
  CHECK(result.t = 9.0f);
}