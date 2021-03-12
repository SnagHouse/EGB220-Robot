#ifndef IOPin_h__
#define IOPin_h__

#include <stdint.h>

/**
 * Register is a helper class for storing and manipulating
 * the Pin/Port/Direction registers of an avr board.
 * 
 * Usage:
 *   Register dOut(PORTD);
 *   Register dDir(DDRD);
 * 
 *   dOut.Set(y, true); // Set output of pin 7 to HIGH
 *   dDir.Set(7, true); // Set direction control pin 7 to HIGH
 **/

class Register
{
  Register(void *offset, int width);

public:
  // Constructors for the supported register types
  // I don't know if avr boads have 32bit registers, but I added it anyway.
  Register(uint32_t &reg);
  Register(uint16_t &reg);
  Register(uint8_t &reg);

  // Get the size of the register in bits
  int GetRegisterWidth();

  // Set a pin in the register.
  void Write(int pin, bool high);

  // Read a pin from the register.
  bool Read(int pin);

  // Clear the register.
  // Optionally specify if it should be cleared to HIGH or LOW.
  // Defaults to LOW.
  void Clear(bool high = false);

protected:
  int m_width = 0; // width of the register in bytes

  // A pointer to the register.
  // A Union is used so dealing with different register widths
  // isn't as painful
  union
  {
    uint32_t* u32;
    uint16_t* u16;
    uint8_t*  u8;
    void* ptr;
  } m_pReg;
};

/**
 * IOPin is a helper class for managing pins on AVR boards
 * using the register macros.
 **/
class IOPin
{
public:
  // Create an IOPin from a a pin number and the associated registers
  IOPin(int pin, Register inputReg, Register outputReg, Register directionReg);

  // Create an IOPin from a pin number and a register char code.
  // The char code identifies the register that the pin is in.
  // This code must be 'a', 'b', 'c' or 'd'
  IOPin(int pin, char registerCode);

  // Read the pin value.
  // Sets the direction of the pin to INPUT
  bool Read();

  // Write a value to the pin.
  // Sets the direction of the pin to OUTPUT
  void Write(bool high);

  // Returns true if this pin is in OUTPUT mode
  bool IsOutput();

protected:
  int m_pin;
  Register m_inputReg;
  Register m_outputReg;
  Register m_directionReg;
};

#endif // IOPin_h__