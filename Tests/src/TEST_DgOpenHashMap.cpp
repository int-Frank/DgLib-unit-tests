#include <iostream>

#include "TestHarness.h"
#include "DgOpenHashMap.h"

TEST(Stack_DgHashMapConstruction, creation_DgHashMapConstruction)
{
  Dg::OpenHashMap<int, int> map;
  CHECK(map.size() == 0);
  CHECK(map.bucket_count() == Dg::impl::OpenHashMap::defaultBucketCount);
  CHECK(map.empty() == true);
  CHECK(map.max_load_factor() == Dg::impl::OpenHashMap::defaultLoadFactor);
  CHECK(map.load_factor() == 0.0);
}

TEST(Stack_DgHashMapInsert, creation_DgHashMapInsert)
{
  Dg::OpenHashMap<int, int> map;

  map.insert(112, 1);
  map.insert(11, 2);
  map.insert(543, 3);
  map.insert(7345, 4);

  int* pval = map.at(112);
  CHECK(pval != nullptr);
  CHECK(*pval == 1);
  CHECK(map[112] == 1);

  pval = map.at(11);
  CHECK(pval != nullptr);
  CHECK(*pval == 2);
  CHECK(map[11] == 2);

  pval = map.at(543);
  CHECK(pval != nullptr);
  CHECK(*pval == 3);
  CHECK(map[543] == 3);

  pval = map.at(7345);
  CHECK(pval != nullptr);
  CHECK(*pval == 4);
  CHECK(map[7345] == 4);

  pval = map.at(2);
  CHECK(pval == nullptr);

  map[9293] = 7;
  pval = map.at(9293);
  CHECK(pval != nullptr);
  CHECK(*pval == 7);

  Dg::OpenHashMap<int, int> const * pmap = &map;

  int const * cpval = pmap->at(112);
  CHECK(cpval != nullptr);
  CHECK(*cpval == 1);

  cpval = pmap->at(11);
  CHECK(cpval != nullptr);
  CHECK(*cpval == 2);

  cpval = pmap->at(543);
  CHECK(cpval != nullptr);
  CHECK(*cpval == 3);

  cpval = pmap->at(7345);
  CHECK(cpval != nullptr);
  CHECK(*cpval == 4);

  cpval = pmap->at(2);
  CHECK(cpval == nullptr);
}

TEST(Stack_DgHashMapCopy, creation_DgHashMapCopy)
{
  Dg::OpenHashMap<int, int> map;
  Dg::OpenHashMap<int, int> mapAssigned;

  map.insert(112, 1);
  map.insert(11, 2);
  map.insert(543, 3);
  map.insert(7345, 4);

  mapAssigned = map;
  Dg::OpenHashMap<int, int>::iterator it = map.begin();
  Dg::OpenHashMap<int, int>::iterator itAssigned = mapAssigned.begin();

  CHECK(map.size() == mapAssigned.size());

  CHECK(it->first == itAssigned->first);
  CHECK(it->second == itAssigned->second);
  it++;
  itAssigned++;

  CHECK(it->first == itAssigned->first);
  CHECK(it->second == itAssigned->second);
  it++;
  itAssigned++;

  CHECK(it->first == itAssigned->first);
  CHECK(it->second == itAssigned->second);
  it++;
  itAssigned++;

  CHECK(it->first == itAssigned->first);
  CHECK(it->second == itAssigned->second);
  it++;
  itAssigned++;
}

TEST(Stack_DgHashMapErase, creation_DgHashMapErase)
{
  Dg::OpenHashMap<int, int> map;
  map.set_buckets(23);

  map.insert(112, 1);
  map.insert(11, 2);
  map.insert(543, 3);
  map.insert(7345, 4);

  map.erase(112);
  int* pval = map.at(112);
  CHECK(pval == nullptr);

  map.erase(11);
  pval = map.at(11);
  CHECK(pval == nullptr);

  map.erase(543);
  pval = map.at(543);
  CHECK(pval == nullptr);

  map.erase(7345);
  pval = map.at(7345);
  CHECK(pval == nullptr);
}

TEST(Stack_DgHashMapRehash, creation_DgHashMapRehash)
{
  Dg::OpenHashMap<int, int> map;

  map.insert(112, 1);
  map.insert(11, 2);
  map.insert(543, 3);
  map.insert(7345, 4);

  map.set_buckets(23);
}