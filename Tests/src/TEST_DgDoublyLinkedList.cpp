#include "TestHarness.h"
#define DEBUG
#include "DgDoublyLinkedList.h"
#include <list>

//TODO Test operator+/operator-
typedef int t;
typedef std::list<t>                   list;
typedef Dg::DoublyLinkedList<t>        DgList;

//#define BRK do{char t(0); std::cin >> t;}while(false)
//
//#include <iostream>
//void Print(std::list<t> const & a_list)
//{
//  std::cout << "std::list = ";
//  for (auto val : a_list)
//    std::cout << val << ", ";
//  std::cout << "\n";
//}
//
//void Print(DgList const & a_list)
//{
//  std::cout << "DgList    = ";
//  for (auto it = a_list.cbegin(); it != a_list.cend(); it++)
//    std::cout << *it << ", ";
//  std::cout << "\n";
//}

bool CheckState(list & a_list, DgList & a_dgList)
{
  if (a_list.size() != a_dgList.size())
  {
    return false;
  }

  DgList::const_iterator cdgit = a_dgList.cbegin();
  list::const_iterator clit = a_list.cbegin();
  for (clit, cdgit; clit != a_list.cend(); ++clit, ++cdgit)
  {
    if (*clit != *cdgit) return false;
  }

  DgList::iterator dgit = a_dgList.begin();
  list::iterator lit = a_list.begin();
  for (lit, dgit; lit != a_list.end(); ++lit, ++dgit)
  {
    if (*lit != *dgit) return false;
  }

  if (a_list.size() > 0)
  {
    lit = a_list.end(); --lit;
    dgit = a_dgList.end(); --dgit;
    if (*lit != *dgit) return false;

    clit = a_list.cend(); --clit;
    cdgit = a_dgList.cend(); --cdgit;
    if (*clit != *cdgit) return false;

    if (a_list.back() != a_dgList.back()) return false;
    if (a_list.front() != a_dgList.front()) return false;
  }


  return true;
}

TEST(Stack_DgDoublyLinkedList_iterator1, creation_DgDoublyLinkedList_iterator1)
{
  DgList list;
  for (int i = 0; i < 10; i++)
    list.push_back(i);

  DgList::iterator it = list.begin();
  CHECK(*it == 0);

  it = it + 4;
  CHECK(*it == 4);

  it = it - 2;
  CHECK(*it == 2);

  DgList::const_iterator cit = list.cbegin();
  CHECK(*cit == 0);

  cit = cit + 4;
  CHECK(*cit == 4);

  cit = cit - 2;
  CHECK(*cit == 2);
}

TEST(Stack_DgDoublyLinkedList_iterator2, creation_DgDoublyLinkedList_iterator2)
{
  DgList list;
  for (int i = 0; i < 10; i++)
    list.push_back(i);

  DgList::iterator it = list.begin();
  CHECK(*it == 0);

  it += 4;
  CHECK(*it == 4);

  it -= 2;
  CHECK(*it == 2);

  DgList::const_iterator cit = list.cbegin();
  CHECK(*cit == 0);

  cit += 4;
  CHECK(*cit == 4);

  cit -= 2;
  CHECK(*cit == 2);
}

//--------------------------------------------------------------------------------
//	Dg::map_s
//--------------------------------------------------------------------------------
TEST(Stack_DgDoublyLinkedList, creation_DgDoublyLinkedList)
{
  DgList    dglst;
  list      lst;

  CHECK(dglst.size() == 0);
  CHECK(dglst.empty() == true);

  CHECK(CheckState(lst, dglst));

  t tMax = 20;
  for (t i = 0; i < tMax; ++i)
  {
    dglst.push_back(i);
    dglst.pop_back();
    dglst.push_back(i);
    lst.push_back(i);
    CHECK(CheckState(lst, dglst));
  }

  for (t i = 0; i < tMax; ++i)
  {
    dglst.pop_back();
    lst.pop_back();
    CHECK(CheckState(lst, dglst));
  }

  for (t i = 300; i < 300 + tMax; ++i)
  {
    dglst.push_front(i);
    lst.push_front(i);
    CHECK(CheckState(lst, dglst));
  }

  for (t i = 0; i < tMax; ++i)
  {
    dglst.pop_front();
    lst.pop_front();
    CHECK(CheckState(lst, dglst));
  }

  for (t i = 0; i < tMax; ++i)
  {
    dglst.push_front(i);
    lst.push_front(i);
  }

  list::iterator lit = lst.begin();
  DgList::iterator dgit = dglst.begin();

  lit = lst.insert(lit, 2);
  dgit = dglst.insert(dgit, 2);
  CHECK(*lit == *dgit);
  CHECK(CheckState(lst, dglst));

  lit++; ++lit;
  dgit++; ++dgit;
  lit = lst.insert(lit, 23);
  dgit = dglst.insert(dgit, 23);

  CHECK(*lit == *dgit);
  CHECK(CheckState(lst, dglst));

  lit++; ++lit;
  dgit++; ++dgit;
  lit = lst.erase(lit);
  dgit = dglst.erase(dgit);

  CHECK(*lit == *dgit);
  CHECK(CheckState(lst, dglst));

  lit++; ++lit;
  dgit++; ++dgit;
  lit = lst.erase(lit);
  dgit = dglst.erase(dgit);
  CHECK(*lit == *dgit);
  CHECK(CheckState(lst, dglst));

  lit--; --lit;
  dgit--; --dgit;
  CHECK(*lit == *dgit);
  CHECK(CheckState(lst, dglst));

  lst.clear();
  dglst.clear();
  CHECK(CheckState(lst, dglst));

  for (t i = 0; i < tMax; ++i)
  {
    dglst.push_back(i);
    lst.push_back(i);
    CHECK(CheckState(lst, dglst));
  }

  DgList newlst(dglst);
  DgList newlst2 = dglst;
  
  CHECK(CheckState(lst, newlst));
  CHECK(CheckState(lst, newlst2));

  DgList newlst3(666);
  CHECK(newlst3.size() == 0);
  newlst3 = dglst;
  CHECK(CheckState(lst, newlst3));
}

TEST(Stack_DgDoublyLinkedList_sort, creation_DgDoublyLinkedList_sort)
{
  DgList list;
  list.push_back(6);
  list.push_back(8);
  list.push_back(3);
  list.push_back(9);
  list.push_back(4);
  list.push_back(0);
  list.push_back(5);
  list.push_back(7);
  list.push_back(2);
  list.push_back(1);

  list.sort();

  DgList::iterator it = list.begin();
  CHECK(*it == 0);
  it++; CHECK(*it == 1);
  it++; CHECK(*it == 2);
  it++; CHECK(*it == 3);
  it++; CHECK(*it == 4);
  it++; CHECK(*it == 5);
  it++; CHECK(*it == 6);
  it++; CHECK(*it == 7);
  it++; CHECK(*it == 8);
  it++; CHECK(*it == 9);
  it++; CHECK(it == list.end());
}