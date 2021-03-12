#include "Register.h"

Register::Register(volatile void *offset, int width)
{
  m_pReg.ptr = offset;
  m_width = width;
}

// Just forward the address of the register and it's size to the private constructor
Register::Register(volatile uint32_t &reg) : Register(&reg, sizeof(reg)) {}
Register::Register(volatile uint16_t &reg) : Register(&reg, sizeof(reg)) {}
Register::Register(volatile uint8_t &reg)  : Register(&reg, sizeof(reg)) {}

int Register::GetRegisterWidth() { return m_width * sizeof(uint8_t); }

// Using a template to set pins so I don't have to repeat this for all register types.
template<typename T>
void SetPin(volatile T *pReg, uint8_t pin, bool high)
{
  if (high) *pReg |= T(1 << pin);  // Set the bit at 'pin' to 1
  else      *pReg &= ~T(1 << pin); // Set the bit at 'pin' to 0
}

template<typename T>
bool GetPin(volatile T *pReg, uint8_t pin)
{
  return ((*pReg) >> pin) & 1; // Shift the pin value to the LSB and AND with 0b00000001
}

template<typename T>
void SetRegister(volatile T *pReg, bool high)
{
  *pReg = high ? ~T(0) : T(0);
}

void Register::Write(uint8_t pin, bool high)
{
  switch (m_width)
  {
  case 4: SetPin(m_pReg.u32, pin, high); break;
  case 2: SetPin(m_pReg.u16, pin, high); break;
  case 1: SetPin(m_pReg.u8,  pin, high); break;
  }
}

bool Register::Read(uint8_t pin)
{
  switch (m_width)
  {
  case 4: return GetPin(m_pReg.u32, pin);
  case 2: return GetPin(m_pReg.u16, pin);
  case 1: return GetPin(m_pReg.u8,  pin);
  }

  // Bad register width... This should never happen...
  return false;
}

void Register::Clear(bool high)
{
  switch (m_width)
  {
  case 4: SetRegister(m_pReg.u32, high); break;
  case 2: SetRegister(m_pReg.u16, high); break;
  case 1: SetRegister(m_pReg.u8,  high); break;
  }
}

// Read the entire register
void Read(uint8_t *pReg)
{
}

void Read(uint16_t *pReg)
{
}

void Read(uint32_t *pReg)
{
}

// Set the register
void Set(uint8_t reg)
{
}

void Set(uint16_t reg)
{
}

void Set(uint32_t reg)
{
}
