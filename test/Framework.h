#ifndef Framework_h__
#define Framework_h__

#include <functional>

int  Tests_Run();
int  Tests_Add(char const * name, std::function<void()> func);
void Tests_Fail(char const * message, char const * file, int line);
bool Tests_Call(std::function<bool()> func);

#define Test_Assert(condition) Tests_Call([&]() {\
  if (!(condition)) {\
    Tests_Fail("Assertion Failed: " #condition, __FILE__, __LINE__);\
    return false;\
  }\
  return true;\
})

#define Test(name)\
void name();\
static int __Framework_Test_ ## name ## _Status = Tests_Add(#name, name);\
static void name()\

#endif // Framework_h__
