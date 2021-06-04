#include <map>
#include <stdio.h>

#include "TestHarness.h"
#include "DgStaticBoolArray.h"

TEST(Stack_DgStaticBoolArray, creation_DgStaticBoolArray)
{
  Dg::StaticBoolArray<127> ary;
  bool val = false;

  for (size_t i = 0; i < ary.Size(); i++)
  {
    CHECK(ary.Get(i, val) == Dg::ErrorCode::None);
    CHECK(ary[i] == false);
    CHECK(val == false);
  }

  ary.SetAllOn();
  for (size_t i = 0; i < ary.Size(); i++)
  {
    CHECK(ary.Get(i, val) == Dg::ErrorCode::None);
    CHECK(ary[i] == true);
    CHECK(val == true);
  }

  ary.SetAllOff();
  for (size_t i = 0; i < ary.Size(); i++)
  {
    CHECK(ary.Get(i, val) == Dg::ErrorCode::None);
    CHECK(ary[i] == false);
    CHECK(val == false);
  }

  ary.SetAll(true);
  for (size_t i = 0; i < ary.Size(); i++)
  {
    CHECK(ary.Get(i, val) == Dg::ErrorCode::None);
    CHECK(ary[i] == true);
    CHECK(val == true);
  }

  ary.SetAll(false);
  for (size_t i = 0; i < ary.Size(); i++)
  {
    CHECK(ary.Get(i, val) == Dg::ErrorCode::None);
    CHECK(ary[i] == false);
    CHECK(val == false);
  }

  for (size_t ind = 0; ind < ary.Size(); ind++)
  {
    ary.SetAllOff();
    CHECK(ary.Set(ind, true) == Dg::ErrorCode::None);
    for (size_t i = 0; i < ary.Size(); i++)
    {
      bool res = i == ind ? true : false;
      CHECK(ary.Get(i, val) == Dg::ErrorCode::None);
      CHECK(ary[i] == res);
      CHECK(val == res);
    }

    CHECK(ary.Toggle(ind) == Dg::ErrorCode::None);
    for (size_t i = 0; i < ary.Size(); i++)
    {
      CHECK(ary.Get(i, val) == Dg::ErrorCode::None);
      CHECK(ary[i] == false);
      CHECK(val == false);
    }

    CHECK(ary.Toggle(ind) == Dg::ErrorCode::None);
    for (size_t i = 0; i < ary.Size(); i++)
    {
      bool res = i == ind ? true : false;
      CHECK(ary.Get(i, val) == Dg::ErrorCode::None);
      CHECK(ary[i] == res);
      CHECK(val == res);
    }

    CHECK(ary.SetOff(ind) == Dg::ErrorCode::None);
    for (size_t i = 0; i < ary.Size(); i++)
    {
      CHECK(ary.Get(i, val) == Dg::ErrorCode::None);
      CHECK(ary[i] == false);
      CHECK(val == false);
    }

    CHECK(ary.SetOn(ind) == Dg::ErrorCode::None);
    for (size_t i = 0; i < ary.Size(); i++)
    {
      bool res = i == ind ? true : false;
      CHECK(ary.Get(i, val) == Dg::ErrorCode::None);
      CHECK(ary[i] == res);
      CHECK(val == res);
    }
  }

  CHECK(ary.Toggle(ary.Size()) == Dg::ErrorCode::OutOfBounds);
  CHECK(ary.SetOn(ary.Size()) == Dg::ErrorCode::OutOfBounds);
  CHECK(ary.SetOff(ary.Size()) == Dg::ErrorCode::OutOfBounds);
  CHECK(ary.Set(ary.Size(), true) == Dg::ErrorCode::OutOfBounds);
  CHECK(ary.Get(ary.Size(), val) == Dg::ErrorCode::OutOfBounds);
}