#include "TestHarness.h"
#include "DgDynamicArray.h"
#include <vector>

template<typename T>
bool CheckState(std::vector<T> & a_vec, Dg::DynamicArray<T> & a_dgVec)
{
  if (a_vec.size() != a_dgVec.size())
  {
    return false;
  }

  if (a_vec.empty() != a_dgVec.empty())
  {
    return false;
  }

  for (size_t i = 0; i < a_dgVec.size(); ++i)
  {
    if (a_vec[i] != a_dgVec[i])
    {
      return false;
    }
  }

  if (a_vec.size() != 0)
  {
    if (a_vec.back() != a_dgVec.back())
    {
      return false;
    }
  }

  return true;
}


TEST(Stack_dg_DynamicArray, creation_dg_DynamicArray)
{
  std::vector<int>       lst;
  Dg::DynamicArray<int>  dglst;

  CHECK(dglst.size() == 0);
  CHECK(dglst.empty() == true);

  CHECK(CheckState(lst, dglst));

  int tMax = 20;
  for (int i = 0; i < tMax; ++i)
  {
    dglst.push_back(i);
    lst.push_back(i);
    CHECK(CheckState(lst, dglst));
  }

  lst.clear();
  dglst.clear();
  CHECK(CheckState(lst, dglst));

  for (int i = 0; i < tMax; ++i)
  {
    dglst.push_back(i);
    lst.push_back(i);
    CHECK(CheckState(lst, dglst));
  }

  for (int i = 0; i < tMax; ++i)
  {
    dglst.pop_back();
    lst.pop_back();
    CHECK(CheckState(lst, dglst));
  }

  for (int i = 0; i < tMax; ++i)
  {
    dglst.push_back(i);
    lst.push_back(i);
    CHECK(CheckState(lst, dglst));
  }

  Dg::DynamicArray<int> newlst(dglst);
  Dg::DynamicArray<int> newlst2 = dglst;
  CHECK(CheckState(lst, newlst));
  CHECK(CheckState(lst, newlst2));

  Dg::DynamicArray<int> newlst3(666);
  CHECK(newlst3.size() == 0);
  newlst3 = dglst;
  CHECK(CheckState(lst, newlst3));
}

TEST(Stack_dg_DynamicArray_it, creation_dg_DynamicArray_it)
{
  Dg::DynamicArray<int>  dgDA;
  for (int i = 0; i < 10; i++)
    dgDA.push_back(i);

  Dg::DynamicArray<int>::iterator it = dgDA.begin();
  CHECK(*it == 0);

  ++it;
  it++;
  CHECK(*it == 2);

  it += 2;
  CHECK(*it == 4);

  it = it + 4;
  CHECK(*it == 8);

  it--;
  --it;
  CHECK(*it == 6);

  it -= 2;
  CHECK(*it == 4);

  it = it - 2;
  CHECK(*it == 2);

  CHECK(*(--dgDA.end()) == 9);
}

TEST(Stack_dg_DynamicArray_cit, creation_dg_DynamicArray_cit)
{
  Dg::DynamicArray<int>  dgDA;
  for (int i = 0; i < 10; i++)
    dgDA.push_back(i);

  Dg::DynamicArray<int>::const_iterator cit = dgDA.cbegin();
  CHECK(*cit == 0);

  ++cit;
  cit++;
  CHECK(*cit == 2);

  cit += 2;
  CHECK(*cit == 4);

  cit = cit + 4;
  CHECK(*cit == 8);

  cit--;
  --cit;
  CHECK(*cit == 6);

  cit -= 2;
  CHECK(*cit == 4);

  cit = cit - 2;
  CHECK(*cit == 2);

  CHECK(*(--dgDA.cend()) == 9);
}

TEST(Stack_Dg_DynamicArray, Dg_DynamicArray_insert)
{
  std::vector<int>       lst;
  Dg::DynamicArray<int>  dglst;

  int i = 0;
  lst.insert(lst.begin(), i);
  dglst.insert(0, i++);
  CHECK(CheckState(lst, dglst));

  lst.insert(lst.begin(), i);
  dglst.insert(0, i++);
  CHECK(CheckState(lst, dglst));

  lst.insert(lst.begin() + 2, i);
  dglst.insert(2, i++);
  CHECK(CheckState(lst, dglst));

  lst.insert(lst.begin() + 1, i);
  dglst.insert(1, i++);
  CHECK(CheckState(lst, dglst));

  lst.insert(lst.begin(), i);
  dglst.insert(0, i++);
  CHECK(CheckState(lst, dglst));

  lst.insert(lst.begin() + 2, i);
  dglst.insert(2, i++);
  CHECK(CheckState(lst, dglst));

}