#include "Framework.h"
#include "Commands.h"

Test(Commands_Variable)
{
  Commands cmds;
  cmds.addVariable<int>("var");

  Test_Assert(cmds.hasVariable("var"));
  Test_Assert(cmds.set<int>("var", 20));

  int var = 0;
  Test_Assert(cmds.get<int>("var", &var));
  Test_Assert(var == 20);
}

Test(Commands_AddVariableExternal)
{
  int var = 0;
  Commands cmds;
  cmds.addVariable("var", &var);

  Test_Assert(cmds.set<int>("var", 10));
  Test_Assert(var == 10);
}

Test(Commands_VariableExternal)
{
  int var = 0;
  Commands cmds;
  cmds.addVariable("var", &var);

  Test_Assert(cmds.hasVariable("var"));
  Test_Assert(cmds.set<int>("var", 10));
  Test_Assert(var == 10);

  int val;
  Test_Assert(cmds.get<int>("var", &val));
  Test_Assert(val == 10);
}

static uint32_t test = 0;

Test(Commands_Command)
{
  Commands cmds;
  Test_Assert(cmds.addCommand("inc-test", []() { test++; }));
  Test_Assert(cmds.addCommand("dec-test", []() { test--; }));

  Test_Assert(cmds.hasCommand("inc-test"));
  Test_Assert(cmds.hasCommand("dec-test"));

  Test_Assert(test == 0);
  cmds.call("inc-test");
  Test_Assert(test == 1);
  cmds.call("dec-test");
  Test_Assert(test == 0);
}
