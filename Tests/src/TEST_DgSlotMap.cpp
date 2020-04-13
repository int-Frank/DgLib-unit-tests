#include "TestHarness.h"

#include "DgSlotMap.h"

typedef Dg::SlotMap<int> SlotMap;

TEST(Stack_DgSlotMap_constructors, creation_DgSlotMap_constructors)
{
  {
    SlotMap map;
    CHECK(map.size() == 0);
  }

  {
    SlotMap map(32);
    CHECK(map.size() == 0);
  }
}

TEST(Stack_DgSlotMap_copy, creation_DgSlotMap_copy)
{
  SlotMap map;
  int const nItems = 8;

  for (int i = 0; i < 8; i++)
    map.insert(i);

  SlotMap map_construct(map);
  SlotMap map_assignment(map);

  CHECK(map_construct.size() == nItems);
  CHECK(map_construct[0] == 0);
  CHECK(map_construct[1] == 1);
  CHECK(map_construct[2] == 2);
  CHECK(map_construct[3] == 3);
  CHECK(map_construct[4] == 4);
  CHECK(map_construct[5] == 5);
  CHECK(map_construct[6] == 6);
  CHECK(map_construct[7] == 7);

  CHECK(map_assignment.size() == nItems);
  CHECK(map_assignment[0] == 0);
  CHECK(map_assignment[1] == 1);
  CHECK(map_assignment[2] == 2);
  CHECK(map_assignment[3] == 3);
  CHECK(map_assignment[4] == 4);
  CHECK(map_assignment[5] == 5);
  CHECK(map_assignment[6] == 6);
  CHECK(map_assignment[7] == 7);

  SlotMap map_move = std::move(map);

  CHECK(map.size() == 0);

  CHECK(map_move.size() == nItems);
  CHECK(map_move[0] == 0);
  CHECK(map_move[1] == 1);
  CHECK(map_move[2] == 2);
  CHECK(map_move[3] == 3);
  CHECK(map_move[4] == 4);
  CHECK(map_move[5] == 5);
  CHECK(map_move[6] == 6);
  CHECK(map_move[7] == 7);

}

TEST(Stack_DgSlotMap_modify, creation_DgSlotMap_modify)
{
  SlotMap map;
  int const nItems = 8;

  SlotMap::Key keys[nItems];

  for (int i = 0; i < 8; i++)
    keys[i] = map.insert(i);

  CHECK(map.size() == nItems);
  CHECK(map[0] == 0);
  CHECK(map[1] == 1);
  CHECK(map[2] == 2);
  CHECK(map[3] == 3);
  CHECK(map[4] == 4);
  CHECK(map[5] == 5);
  CHECK(map[6] == 6);
  CHECK(map[7] == 7);

  map.erase(keys[3]);
  CHECK(map.size() == 7);
  CHECK(map[0] == 0);
  CHECK(map[1] == 1);
  CHECK(map[2] == 2);
  CHECK(map[3] == 7);
  CHECK(map[4] == 4);
  CHECK(map[5] == 5);
  CHECK(map[6] == 6);

  map.erase(keys[6]);
  CHECK(map.size() == 6);
  CHECK(map[0] == 0);
  CHECK(map[1] == 1);
  CHECK(map[2] == 2);
  CHECK(map[3] == 7);
  CHECK(map[4] == 4);
  CHECK(map[5] == 5);

  map.erase(keys[0]);
  CHECK(map.size() == 5);
  CHECK(map[0] == 5);
  CHECK(map[1] == 1);
  CHECK(map[2] == 2);
  CHECK(map[3] == 7);
  CHECK(map[4] == 4);

  map.erase(keys[2]);
  CHECK(map.size() == 4);
  CHECK(map[0] == 5);
  CHECK(map[1] == 1);
  CHECK(map[2] == 4);
  CHECK(map[3] == 7);

  map.erase(keys[1]);
  CHECK(map.size() == 3);
  CHECK(map[0] == 5);
  CHECK(map[1] == 7);
  CHECK(map[2] == 4);

  map.erase(keys[1]);
  CHECK(map.size() == 3);

  map.erase(keys[7]);
  CHECK(map.size() == 2);
  CHECK(map[0] == 5);
  CHECK(map[1] == 4);

  map.erase(keys[4]);
  CHECK(map.size() == 1);
  CHECK(map[0] == 5);

  map.erase(keys[5]);
  CHECK(map.size() == 0);

  for (int i = 0; i < 8; i++)
    keys[i] = map.insert(i);

  CHECK(map.size() == nItems);
  CHECK(map[0] == 0);
  CHECK(map[1] == 1);
  CHECK(map[2] == 2);
  CHECK(map[3] == 3);
  CHECK(map[4] == 4);
  CHECK(map[5] == 5);
  CHECK(map[6] == 6);
  CHECK(map[7] == 7);
}

TEST(Stack_DgSlotMap_Extend, creation_DgSlotMap_Extend)
{
  SlotMap map(4);

  for (int i = 0; i < 1024 * 1024; i++)
    map.insert(i);

  for (int i = 0; i < 1024 * 1024; i++)
    CHECK(map[i] == i);
}

TEST(Stack_DgSlotMap_iterator, creation_DgSlotMap_iterator)
{
  SlotMap map;

  for (int i = 0; i < 4; i++)
    map.insert(i);

  SlotMap::iterator it = map.begin();
  CHECK(*it == 0);
  it++; CHECK(*it == 1);
  it++; CHECK(*it == 2);
  it++; CHECK(*it == 3);
  it++; CHECK(it == map.end());
  it--; CHECK(*it == 3);
  it--; CHECK(*it == 2);
  it--; CHECK(*it == 1);
  it--; CHECK(*it == 0);

  SlotMap::const_iterator cit = it;
  CHECK(*cit == 0);
  cit++; CHECK(*cit == 1);
  cit++; CHECK(*cit == 2);
  cit++; CHECK(*cit == 3);
  cit++; CHECK(cit == map.cend());
  cit--; CHECK(*cit == 3);
  cit--; CHECK(*cit == 2);
  cit--; CHECK(*cit == 1);
  cit--; CHECK(*cit == 0);
}