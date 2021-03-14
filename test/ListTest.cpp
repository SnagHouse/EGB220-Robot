#include "Framework.h"
#include "List.h"

Test(List_ConstructDefault)
{
  List<int> list;
  Test_Assert(list.data() == nullptr);
  Test_Assert(list.capacity() == 0);
  Test_Assert(list.size() == 0);
}

Test(List_ConstructCapacity)
{
  List<int> list(10);
  int a = utilMin(10, 11);
  Test_Assert(list.data() != nullptr);
  Test_Assert(list.capacity() == 10);
  Test_Assert(list.size() == 0);
}

Test(List_Add)
{
  List<int> l;

  l.add(10);
  Test_Assert(l.size() == 1);
  Test_Assert(l.at(0) == 10);


  l.add(20);
  Test_Assert(l.size() == 2);
  Test_Assert(l.at(0) == 10);
  Test_Assert(l.size() == 2);
  Test_Assert(l.at(1) == 20);
}

Test(List_At)
{
  List<int> l;
  for (int i = 0; i < 10; ++i)
    l.add(i);

  for (int i = 0; i < 10; ++i)
    Test_Assert(l.at(i) == i);
}
