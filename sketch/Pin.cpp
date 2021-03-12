#include "Pin.h"
#include "avr/io.h"

DigitalPin::DigitalPin(uint8_t pin, Register inputReg, Register outputReg, Register directionReg)
  : m_inputReg(inputReg)
  , m_outputReg(outputReg)
  , m_directionReg(directionReg)
  , m_pin(pin)
{}

DigitalPin::DigitalPin(uint8_t pin, char registerCode)
  : m_inputReg(PINB)
  , m_outputReg(PORTB)
  , m_directionReg(DDRB)
  , m_pin(pin)
{
  switch (registerCode)
  {
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

bool DigitalPin::Read()
{
  m_directionReg.Write(m_pin, true); // Set to an input pin
  m_inputReg.Read(m_pin);            // Read the pin value
}

void DigitalPin::Write(bool high)
{
  m_directionReg.Write(m_pin, true); // Set to an output pin
  m_outputReg.Write(m_pin, high);    // Write the pin state
}

bool DigitalPin::IsOutput()
{
  return m_directionReg.Read(m_pin);
}