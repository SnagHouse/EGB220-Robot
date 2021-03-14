#ifndef SerialCommands_h__
#define SerialCommands_h__

#include "Str.h"

// Simple way to get a unique integer for each type
static uint32_t __NextTypeID() { static uint32_t nextID; return nextID++; }

template<typename T> uint32_t TypeID() { static uint32_t id = __NextTypeID(); return id; }

/**
 * Commands class for calling functions and accessing variables using strings.
 */
class Commands
{
public:
  class IVariable
  {
  public:
    virtual ~IVariable() {};
    virtual const Str& getName() = 0;
    virtual void get(void* pData) = 0;
    virtual void set(void const * pData) = 0;
    virtual uint32_t getType() = 0;

    template<typename T>
    bool is() { return getType() == TypeID<T>(); }
  };

  class ICommand
  {
  public:
    virtual ~ICommand() {};
    virtual const Str& getName() = 0;
    virtual void operator()() = 0;
  };

  Commands() = default;
  ~Commands();

  bool call(Str const & name);
  bool hasCommand(Str const & name);
  bool hasVariable(Str const & name);

  template<typename T>
  bool addCommand(Str const & name, T cmd)
  {
    if (hasCommand(name))
      return false;
    m_commands.add(new Command<T>(name, cmd));
    return true;
  }

  template<typename T>
  void addVariable(Str const & name)
  {
    m_variables.add(new Variable<T>(name));
  }

  template<typename T>
  void addVariable(Str const & name, T *pVariable)
  {
    m_variables.add(new Variable<T>(name, pVariable));
  }

  template<typename T>
  bool set(Str const & name, T const & var)
  {
    IVariable *pAccessor = getVariable(name);
    if (!pAccessor || !pAccessor->is<T>())
      return false;
    pAccessor->set(&var);
    return true;
  }

  template<typename T>
  bool get(Str const & name, T * pVar)
  {
    IVariable *pAccessor = getVariable(name);
    if (!pAccessor || !pAccessor->is<T>())
      return false;
    pAccessor->get(pVar);
    return true;
  }

protected:
  template<typename T>
  class Variable : public IVariable
  {
  public:
    Variable(Str var, T *pVar = nullptr)
      : m_name(move(var))
      , m_pData(pVar)
    {
      if (!m_pData)
      {
        m_pData = new T;
        m_ownsVar = true;
      }
    }

    ~Variable()
    {
      if (m_ownsVar)
        delete m_pData;
      m_pData = nullptr;
      m_ownsVar = false;
    }

    virtual void get(void * pData)       override { *(T*)pData = *m_pData; }
    virtual void set(void const * pData) override { *m_pData = *(T*)pData; }
    virtual uint32_t getType()           override { return TypeID<T>(); }
    virtual const Str& getName()         override { return m_name; }

  protected:
    bool m_ownsVar = false;
    T *m_pData = nullptr;
    Str m_name;
  };

  template<typename T>
  class Command : public ICommand
  {
  public:
    Command(Str name, T func)
      : m_name(move(name))
      , m_func(func)
    {}

    virtual const Str& getName() override { return m_name; }
    virtual void operator()()    override { m_func(); }

  protected:
    T m_func = nullptr;
    Str m_name;
  };

  IVariable* getVariable(Str const & name)
  {
    for (IVariable *pVar : m_variables)
      if (pVar->getName().compare(name))
        return pVar;
    return nullptr;
  }

  ICommand* getCommand(Str const & name)
  {
    for (ICommand *pCmd : m_commands)
      if (pCmd->getName().compare(name))
        return pCmd;
    return nullptr;
  }

  List<ICommand*>  m_commands;
  List<IVariable*> m_variables;
};

#endif // SerialCommands_h__
