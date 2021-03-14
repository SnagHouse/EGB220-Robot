#include "Framework.h"
#include <vector>
#include <string>

using namespace std;

struct TestResult
{
  bool success = true;
  string message;
  string file;
  string line;
};

static vector<pair<string, std::function<void()>>> g_allTests;
static vector<pair<string, TestResult>> g_failed;

static TestResult g_lastTest;

void PrintResult(TestResult const & result);

int Tests_Add(const char *name, std::function<void()> func)
{
  g_allTests.push_back({ name, func });
  return 1;
}

void Tests_Fail(char const * message, char const * file, int line)
{
  g_lastTest.success = false;
  g_lastTest.file = file;
  g_lastTest.message = message;
  g_lastTest.line = to_string(line);
}

bool Tests_Call(std::function<bool()> func) { return func(); }

int Tests_Run()
{
  g_failed.clear();

  printf("-- RUNNING ALL TESTS -- \n\n");

  size_t numPassed = 0;
  size_t testIndex = 0;
  size_t testCount = g_allTests.size();
  for (const pair<string, std::function<void()>> &test : g_allTests)
  {
    ++testIndex;
    g_lastTest = TestResult(); // Clear last result
    test.second(); // Run the test
    printf("[%lld/%lld] %s: %s\n", testIndex, testCount, test.first.c_str(), g_lastTest.success ? "SUCCESS" : "FAILED");
    numPassed += g_lastTest.success;
    if (!g_lastTest.success)
    { // Print reason
      PrintResult(g_lastTest);
      g_failed.emplace_back();
      g_failed.back().first = test.first;
      g_failed.back().second = g_lastTest;
    }
  }

  printf("\n-- ALL TESTS COMPLETED -- \n");
  printf("[%lld/%lld] PASSED\n", numPassed, testCount);

  if (g_failed.size() > 0)
  {
    printf("\n-- FAILED TESTS --\n");
    for (const pair<string, TestResult> &test : g_failed)
    {
      printf("%s : [\n", test.first.c_str());
      PrintResult(test.second);
      printf("]\n");
    }
  }
  return 0;
}

static void PrintResult(TestResult const & result)
{
  printf("  File:    %s\n", result.file.c_str());
  printf("  Line:    %s\n", result.line.c_str());
  printf("  Message: \"%s\"\n", result.message.c_str());
}
