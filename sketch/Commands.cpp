#include "Commands.h"

Commands::~Commands()
{
  for (ICommand *pCmd : m_commands)
    delete pCmd;
  for (IVariable *pVar : m_variables)
    delete pVar;
}

bool Commands::call(Str const & name)
{
  ICommand *pCmd = getCommand(name);
  if (!pCmd)
    return false;

  (*pCmd)();
  return true;
}

bool Commands::hasCommand(Str const & name)
{
  return getCommand(name) != 0;
}

bool Commands::hasVariable(Str const & name)
{
  return getVariable(name) != 0;
}
