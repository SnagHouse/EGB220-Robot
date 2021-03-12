#include "IOPin.h"
#include "avr/io.h"

Register::Register(void *offset, int width)
{
  m_pReg.ptr = offset;
  m_width = width;
}

// Just forward the address of the register and it's size to the private constructor
Register::Register(uint32_t &reg) : Register(&reg, sizeof(reg)) {}
Register::Register(uint16_t &reg) : Register(&reg, sizeof(reg)) {}
Register::Register(uint8_t &reg)  : Register(&reg, sizeof(reg)) {}

int Register::GetRegisterWidth() { return m_width * sizeof(uint8_t); }

// Using a template to set pins so I don't have to repeat this for all
// register types. Templates are generally always inlined, so it shouldn't
// impact the size of the binary
template<typename T> void SetPin(T *pReg, int pin, bool high)
{
  if (high) *pReg |= T(1 << pin);  // Set the bit at 'pin' to 1
  else      *pReg &= ~T(1 << pin); // Set the bit at 'pin' to 0
}

template<typename T> bool GetPin(T *pReg, int pin)
{
  return ((*pReg) >> pin) & 1; // Shift the pin value to the LSB and AND with 0b00000001
}

void Register::Write(int pin, bool high)
{
  switch (m_width)
  {
  case 4: SetPin(m_pReg.u32, pin, high); break;
  case 2: SetPin(m_pReg.u16, pin, high); break;
  case 1: SetPin(m_pReg.u8,  pin, high); break;
  }
}

bool Register::Read(int pin)
{
  switch (m_width)
  {
  case 4: return GetPin(m_pReg.u32, pin);
  case 2: return GetPin(m_pReg.u32, pin);
  case 1: return GetPin(m_pReg.u32, pin);
  }

  // Bad register width... This should never happen...
  return false;
}

IOPin::IOPin(int pin, Register inputReg, Register outputReg, Register directionReg)
  : m_inputReg(inputReg)
  , m_outputReg(outputReg)
  , m_directionReg(directionReg)
  , m_pin(pin)
{}

IOPin::IOPin(int pin, char registerCode)
  : m_inputReg(PINA)
  , m_outputReg(PORTA)
  , m_directionReg(DDRA)
  , m_pin(pin)
{
  switch (registerCode)
  {
  case 'a':
  case 'A': // Allow upper and lower
    m_inputReg = Register(PINA);
    m_inputReg = Register(PORTA);
    m_inputReg = Register(DDRA);
    break;

  case 'b':
  case 'B':
    m_inputReg = Register(PINB);
    m_inputReg = Register(PORTB);
    m_inputReg = Register(DDRB);
    break;

  case 'c':
  case 'C':
    m_inputReg = Register(PINC);
    m_inputReg = Register(PORTC);
    m_inputReg = Register(DDRC);
    break;

  case 'd':
  case 'D':
    m_inputReg = Register(PIND);
    m_inputReg = Register(PORTD);
    m_inputReg = Register(DDRD);
    break;
  }
}

bool IOPin::Read()
{
  m_directionReg.Write(m_pin, true); // Set to an input pin
  m_inputReg.Read(m_pin);            // Read the pin value
}

void IOPin::Write(bool high)
{
  m_directionReg.Write(m_pin, true); // Set to an output pin
  m_outputReg.Write(m_pin, high);    // Write the pin state
}

bool IOPin::IsOutput()
{
  return m_directionReg.Read(m_pin);
}