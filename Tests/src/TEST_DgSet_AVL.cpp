#include <iostream>
#include <set>

#include "TestHarness.h"
#include "DgSet_AVL.h"

//TODO Test operator+/operator-
typedef Dg::Set_AVL<int> Set;
typedef Dg::Set_AVL<int>::iterator             Iter;
typedef Dg::Set_AVL<int>::const_iterator       CIter;
typedef Dg::Set_AVL<int>::iterator_rand        IterR;
typedef Dg::Set_AVL<int>::const_iterator_rand  CIterR;

static int const nItems = 20;
static int const keys[nItems] = {5, 12, 3, 19, 9, 7, 2, 20, 18, 15, 10, 11, 1, 4, 6, 8, 13, 14, 16, 17};

bool AreEqual(Set const & m0, Set const & m1)
{
  if (m0.size() != m1.size())
    return false;

  Set::const_iterator m0_cit = m0.cbegin();
  Set::const_iterator m1_cit = m1.cbegin();
  for (; m0_cit != m0.cend(); m0_cit++, m1_cit++)
  {
    if (*m0_cit != *m1_cit)
      return false;
  }
  return true;
}

struct Data
{
  int v0;
  mutable int v1;
};

bool CustomCompare(Data const & a, Data const & b)
{
  return a.v0 < b.v0;
}

TEST(Stack_dg_AVL, creation_dg_AVLCustom)
{
  Dg::Set_AVL<Data, CustomCompare> set;
  for (int i = 0; i < nItems; i++)
    set.insert({keys[i], i});

  Dg::Set_AVL<Data, CustomCompare>::iterator it = set.find({7, 0});
  CHECK(it != set.end());
  CHECK((*it).v0 == 7);
  CHECK((*it).v1 == 5);
}

TEST(Stack_dg_AVLTreeSeIterator1, creation_dg_AVLTreeSetIterator1)
{
  Set set;
  Set const & rset(set);
  for (int i = 0; i < nItems; i++)
    set.insert(keys[i]);

  Iter it = set.find(7);
  CHECK(it != set.end());
  CHECK(*(it + 2) == 9);
  CHECK(*(it - 2) == 5);

  CIter cit = rset.find(7);
  CHECK(cit != set.cend());
  CHECK(*(cit + 2) == 9);
  CHECK(*(cit - 2) == 5);

  for (int i = 1; i <= 20; i++)
    CHECK(set.exists(i));

  CHECK(!set.exists(-1));
  CHECK(!set.exists(21));
}

TEST(Stack_dg_AVLTreeSetIterator2, creation_dg_AVLTreeSetIterator2)
{
  Set set;
  Set const & rset(set);
  for (int i = 0; i < nItems; i++)
    set.insert(keys[i]);

  Iter it = set.find(7);
  CHECK(it != set.end());

  it += 2;
  CHECK(*it == 9);

  it -= 4;
  CHECK(*it == 5);

  CIter cit = rset.find(7);
  CHECK(cit != set.cend());

  cit += 2;
  CHECK(*cit == 9);

  cit -= 4;
  CHECK(*cit == 5);
}

TEST(Stack_dg_AVLTreeSet, creation_dg_AVLTreeSet)
{
  Set set, set2;

  //----------------------------------------------------------------------------------------
  //Insertion
  //----------------------------------------------------------------------------------------
  for (int i = 0; i < nItems; i++)
    set.insert(keys[i]);

  //----------------------------------------------------------------------------------------
  //Copy
  //----------------------------------------------------------------------------------------
  Set set3(set);
  CHECK(AreEqual(set, set3));
  Set set4 = set3;
  CHECK(AreEqual(set3, set4));

  //----------------------------------------------------------------------------------------
  //Iterating
  //----------------------------------------------------------------------------------------
  int value = 1;
  for (auto v : set3)
  {
    CHECK(v == value);
    value++;
  }

  value = 1;
  for (auto v : set)
  {
    CHECK(v == value);
    value++;
  }

  Set::iterator it = set.end();
  for (int i = 20; i > 0; i--)
  {
    it--;
    CHECK(*it == i);
  }

  value = 1;
  for (Set::iterator it = set.begin(); it != set.end(); it++)
  {
    CHECK(*it == value);
    value++;
  }

  Set::const_iterator cit = set.cend();
  for (int i = 20; i > 0; i--)
  {
    cit--;
    CHECK(*cit == i);
  }

  value = 1;
  for (Set::const_iterator cit = set.cbegin(); cit != set.cend(); cit++)
  {
    CHECK(*cit == value);
    value++;
  }

  //----------------------------------------------------------------------------------------
  //Utility
  //----------------------------------------------------------------------------------------
  CHECK(set.size() == 20);
  CHECK(!set.empty());

  //----------------------------------------------------------------------------------------
  //Accessing/searching
  //----------------------------------------------------------------------------------------
  Set const & crset(set);

  cit = crset.find(4);
  CHECK(cit != crset.cend());

  cit = crset.find(-2);
  CHECK(cit == set.cend());

  cit = crset.find(514);
  CHECK(cit == crset.cend());

  //Non-const tests...
  it = set.find(4);
  CHECK(it != set.end());

  it = set.find(-2);
  CHECK(it == set.end());

  it = set.find(514);
  CHECK(it == set.end());


  //----------------------------------------------------------------------------------------
  //Erasing
  //----------------------------------------------------------------------------------------
  set.clear();

  for (int i = 0; i < nItems; i++)
    set.insert(keys[i]);

  set2 = set;
  for (int i = nItems; i >= 1; i--)
  {
    set.erase(i);

    int value = 1;
    CHECK(set.size() == i-1)
    for (auto v : set)
    {
      CHECK(v == value);
      value++;
    }
  }

  set = set2;
  for (int i = 1; i <= nItems; i++)
  {
    set.erase(i);

    int value = i + 1;
    CHECK(set.size() == nItems - i);
    for (auto v : set)
    {
      CHECK(v == value);
      value++;
    }
  }

  set = set2;
  std::set<int> items;
  for (int i = 1; i <= 20; i++)
    items.insert(i);

  for (int i = 0; i < nItems; i++)
  {
    set.erase(keys[i]);
    items.erase(keys[i]);
    std::set<int>::iterator sit = items.begin();

    for (auto v : set)
    {
      CHECK(v == *sit);
      sit++;
    }
  }

  set = set2;
  it = set.begin();
  for (int i = 1; i < nItems; i++)
  {
    it = set.erase(it);
    CHECK(*it == i+1);
  }

  it = set.erase(it);
  CHECK(it == set.end());

  set = set2;
  it = set.end();
  for (int i = nItems; i > 0; i--)
  {
    it--;
    it = set.erase(it);
    CHECK(it == set.end());
  }

  set = set2;
  it = set.begin();
  for (int i = 0; i < 5; i++)
    it++;

  //Erase 6
  it = set.erase(it);
  CHECK(*it == 7);

  for (int i = 0; i < 2; i++)
    it++;

  //Erase 9
  it = set.erase(it);
  CHECK(*it == 10);

  it--;
  //Erase 8
  it = set.erase(it);
  CHECK(*it == 10);

  set = set2;
  it = set.begin();
  for (int i = 0; i < 8; i++)
    it++;
  //Erase 9 (root)
  it = set.erase(it);
  CHECK(*it == 10);

  set = set2;
  items.clear();
  for (int i = 1; i <= 20; i++)
    items.insert(i);

  //Check deletion of each element only
  for (int i = 0; i < nItems; i++)
  {
    items.erase(keys[i]);
    set.erase(keys[i]);
    std::set<int>::iterator sit = items.begin();
    for (auto v : set)
    {
      CHECK(v == *sit);
      sit++;
    }
    set = set2;
    items.insert(keys[i]);
  }

  //Check deletion of each element only via iterator
  set = set2;
  for (int i = 0; i < nItems; i++)
  {
    it = set.begin();
    for (int j = 0; j < keys[i] - 1; j++)
      it++;

    items.erase(keys[i]);
    it = set.erase(it);

    if (keys[i] == 20)
    {
      CHECK(it == set.end());
    }
    else
    {
      CHECK(*it == keys[i] + 1);
    }

    std::set<int>::iterator sit = items.begin();
    for (auto v : set)
    {
      CHECK(v == *sit);
      sit++;
    }
    set = set2;
    items.insert(keys[i]);
  }

  //----------------------------------------------------------------------------------------
  //Clearing
  //----------------------------------------------------------------------------------------
  set.clear();
  CHECK(set.empty());
}

TEST(Stack_dg_AVLTreeSet, lower_bound_dg_AVLTreeSet)
{
  Dg::Set_AVL<double> set;

  for (int i = 0; i < 100; ++i)
    set.insert(double(i));

  CHECK(set.lower_bound(-1.0) != set.end());
  CHECK(*set.lower_bound(-1.0) == 0.0);

  CHECK(set.lower_bound(99.0) != set.end());
  CHECK(*set.lower_bound(99.0) == 99.0);

  CHECK(set.lower_bound(99.5) == set.end());

  for (int i = 0; i < 99; ++i)
  {
    CHECK(set.lower_bound(double(i) + 0.5) != set.end());
    CHECK(*set.lower_bound(double(i)) == double(i));
    CHECK(*set.lower_bound(double(i) + 0.5) == double(i + 1));
  }
}