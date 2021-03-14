#include "Framework.h"
#include "Str.h"

Test(Str_DefaultConstruct)
{
  Str s;
  
  Test_Assert(s.length() == 0);
  Test_Assert(strcmp(s.data(), "") == 0);
  Test_Assert(*s.end() == 0);
  Test_Assert(*s.begin() == 0);
}

Test(Str_Construct)
{
  Str s("Hello World");

  Test_Assert(s.length() == 11);
  Test_Assert(strcmp(s.data(), "Hello World") == 0);
}

Test(Str_Cat)
{
  Str hello("Hello");
  Str world(" World");

  Test_Assert(hello.cat(world).compare("Hello World"));
}

Test(Str_Find)
{
  Str haystack = "This is a haystack";
  Str needle   = "haystack";

  Test_Assert(haystack.find(needle) == 10);
  Test_Assert(haystack.find('h') == 1);
  Test_Assert(haystack.find(' ') == 4);
}

Test(Str_Join)
{
  List<Str> strings;
  strings.add("one");
  strings.add("two");
  strings.add("three");

  Test_Assert(Str::join(strings, ", ").compare("one, two, three"));
}

Test(Str_Compare)
{
  Str s("Hello World");

  Test_Assert(s.length() == 11);
  Test_Assert(s.compare("Hello World"));
}
