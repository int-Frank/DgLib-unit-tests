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

typedef Dg::BinPkr_ItemID ID;
typedef Dg::BinPacker<int> BPkr;
typedef std::map<ID, MyItem> MyMap;

bool IsContainedIn(MyItem const & a_item, int a_binW, int a_binH)
{
  bool Wgood = a_item.pos[Dg::Element::x] + a_item.dim[Dg::Element::width] <= a_binW;
  bool Hgood = a_item.pos[Dg::Element::y] + a_item.dim[Dg::Element::height] <= a_binH;
  Wgood = Wgood && a_item.pos[Dg::Element::x] >= 0;
  Hgood = Hgood && a_item.pos[Dg::Element::y] >= 0;
  return Wgood &&  Hgood;
}

bool Intersects(MyItem const & a_A, MyItem const & a_B)
{
  bool noIntersects = true;
  noIntersects = noIntersects || (a_A.pos[Dg::Element::x] + a_A.dim[Dg::Element::width] <= a_B.pos[Dg::Element::x]);
  noIntersects = noIntersects || (a_A.pos[Dg::Element::x] >= a_B.pos[Dg::Element::x]+ a_B.dim[Dg::Element::width]);
  noIntersects = noIntersects || (a_A.pos[Dg::Element::y] + a_A.dim[Dg::Element::height] <= a_B.pos[Dg::Element::y]);
  noIntersects = noIntersects || (a_A.pos[Dg::Element::y] >= a_B.pos[Dg::Element::y] + a_B.dim[Dg::Element::height]);
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

TEST(Stack_DgRectanglePacker, creation_DgRectanglePacker)
{
  int binMinRange[2] = {256, 512};
  int binMaxRange[2] = {768, 1024};

  int itemMin = 32;
  int itemMax = 64;

  int nItems = 256;
  int nTrials = 3;

  printf("\nRECTANGLE PACKER\n");

  for (int t = 0; t < nTrials; t++)
  {
    printf("\nTrial %i\n", t);
    Dg::RNG_Local rng;
    rng.SetSeed(t + 13);

    BPkr rp;
    MyMap itemMap;

    for (int i = 0; i < nItems; i++)
    {
      MyItem item;
      item.dim[Dg::Element::width] = rng.GetUintRange(itemMin, itemMax);
      item.dim[Dg::Element::height] = rng.GetUintRange(itemMin, itemMax);

      ID id = rp.RegisterItem(item.dim[Dg::Element::width], item.dim[Dg::Element::height]);
      CHECK(Dg::BinPkr_GetError(id) == Dg::BinPkrError::None);
      itemMap.insert(std::pair<ID, MyItem>(id, item));
    }

    size_t remainder = 0;
    Dg::DynamicArray<BPkr::Bin> binArray;
    do
    {
      BPkr::Bin bin;
      bin.dimensions[0] = rng.GetUintRange(binMinRange[0], binMinRange[1]);
      bin.dimensions[1] = rng.GetUintRange(binMinRange[0], binMinRange[1]);
      bin.maxDimensions[0] = rng.GetUintRange(binMaxRange[0], binMaxRange[1]);
      bin.maxDimensions[1] = rng.GetUintRange(binMaxRange[0], binMaxRange[1]);

      remainder = rp.Fill(bin);
      binArray.push_back(bin);
    } while (remainder > 0);

    CHECK(binArray.size() > 0);

    int binNumber = 0;
    for (auto & bin : binArray)
    {
      CHECK(bin.dimensions[Dg::Element::width] <= bin.maxDimensions[Dg::Element::width]);
      CHECK(bin.dimensions[Dg::Element::height] <= bin.maxDimensions[Dg::Element::height]);

      Dg::DynamicArray<MyItem> items;
      
      for (auto const & item : bin.items)
      {
        ID id = item.id;
        itemMap.at(id).pos[Dg::Element::x] = item.xy[Dg::Element::x];
        itemMap.at(id).pos[Dg::Element::y] = item.xy[Dg::Element::y];
        items.push_back(itemMap.at(id));
      }

      int itemArea = 0;
      for (size_t i = 0; i < items.size(); i++)
      {
        itemArea += items[i].dim[0] * items[i].dim[1];
        CHECK(IsContainedIn(items[i], bin.dimensions[Dg::Element::width], bin.dimensions[Dg::Element::height]));
        CHECK(AvoidsAll(items, i));
      }

      double binArea = bin.dimensions[0] * bin.dimensions[1];
      double used = (double(itemArea) / binArea) * 100.0;
      printf("  Bin %i: %.1f%%\n", binNumber, used);
      binNumber++;
    }
  }
}