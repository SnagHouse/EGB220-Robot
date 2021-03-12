#ifndef Pin_h__
#define Pin_h__

#include "Register.h"

/**
 * IOPin is a helper class for managing pins on AVR boards
 * using the register macros.
 **/
class DigitalPin
{
public:
  // Create an IOPin from a a pin number and the associated registers
  DigitalPin(uint8_t pin, Register inputReg, Register outputReg, Register directionReg);

  // Create an IOPin from a pin number and a register char code.
  // The char code identifies the register that the pin is in.
  // This code must be 'b', 'c' or 'd'
  DigitalPin(uint8_t pin, char registerCode);

  // Read the pin value.
  // Sets the direction of the pin to INPUT
  bool Read();

  // Write a value to the pin.
  // Sets the direction of the pin to OUTPUT
  void Write(bool high);

  // Returns true if this pin is in OUTPUT mode
  bool IsOutput();

protected:
  uint8_t  m_pin;
  Register m_inputReg;
  Register m_outputReg;
  Register m_directionReg;
};

// class AnalogPin
// {
// public:
//   // Create an IOPin from a a pin number and the associated registers
//   AnalogPin(int pin);

//   // Create an IOPin from a pin number and a register char code.
//   // The char code identifies the register that the pin is in.
//   // This code must be 'b', 'c' or 'd'
//   AnalogPin(int pin);

//   // Read the pin value.
//   // Sets the direction of the pin to INPUT
//   bool Read();

//   // Write a value to the pin.
//   // Sets the direction of the pin to OUTPUT
//   void Write(bool high);

//   // Returns true if this pin is in OUTPUT mode
//   bool IsOutput();

// protected:
//   int m_pin;
//   Register m_;
//   Register m_outputReg;
//   Register m_directionReg;
// };

#endif // Pin_h__