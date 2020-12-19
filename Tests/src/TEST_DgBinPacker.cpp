#include <map>
#include <stdio.h>

#include "TestHarness.h"
#include "DgBinPacker.h"

#include "DgDynamicArray.h"
#include "DgRNG_Local.h"

struct MyItem
{
  int dim[2];
  int pos[2];
};

typedef int ID;
typedef Dg::BinPacker<int, ID> BPkr;
typedef std::map<ID, MyItem> MyMap;

bool IsContainedIn(MyItem const & a_item, int a_binW, int a_binH)
{
  bool Wgood = a_item.pos[0] + a_item.dim[0] <= a_binW;
  bool Hgood = a_item.pos[1] + a_item.dim[1] <= a_binH;
  Wgood = Wgood && a_item.pos[0] >= 0;
  Hgood = Hgood && a_item.pos[1] >= 0;
  return Wgood &&  Hgood;
}

bool Intersects(MyItem const & a_A, MyItem const & a_B)
{
  bool noIntersects = true;
  noIntersects = noIntersects || (a_A.pos[0] + a_A.dim[0] <= a_B.pos[0]);
  noIntersects = noIntersects || (a_A.pos[0] >= a_B.pos[0]+ a_B.dim[0]);
  noIntersects = noIntersects || (a_A.pos[1] + a_A.dim[1] <= a_B.pos[1]);
  noIntersects = noIntersects || (a_A.pos[1] >= a_B.pos[1] + a_B.dim[1]);
  return !noIntersects;
}

bool AvoidsAll(Dg::DynamicArray<MyItem> const & a_items, size_t a_index)
{
  MyItem item = a_items[a_index];
  for (size_t i = 0; i < a_items.size(); i++)
  {
    if (i == a_index)
      continue;

    if (Intersects(item, a_items[i]))
      return false;
  }
  return true;
}

TEST(Stack_DgRectanglePacker_Construct, creation_DgRectanglePacker_Construct)
{
  int widthMin = 32;
  int heightMin = 32;
  int widthMax = 64;
  int heightMax = 64;
  
  BPkr rp;

  BPkr rp_copy(rp);
  BPkr rp_assign = rp;
  BPkr rp_move(BPkr());
}