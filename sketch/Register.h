#ifndef Register_h__
#define Register_h__

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
  Register(volatile void *offset, int width);

public:
  // Constructors for the supported register types
  // I don't know if avr boads have 32bit registers, but I added it anyway.
  Register(volatile uint32_t &reg);
  Register(volatile uint16_t &reg);
  Register(volatile uint8_t &reg);

  // Get the size of the register in bits
  int GetRegisterWidth();

  // Set a bit in the register.
  void Write(uint8_t bit, bool high);

  // Read a bit from the register.
  bool Read(uint8_t bit);

  // Clear the register.
  // Optionally specify if it should be cleared to HIGH or LOW.
  // Defaults to LOW.
  void Clear(bool high = false);

  // Read the entire register
  void Read(uint8_t *pReg);
  void Read(uint16_t *pReg);
  void Read(uint32_t *pReg);

  // Set the register
  void Set(uint8_t reg);
  void Set(uint16_t reg);
  void Set(uint32_t reg);

protected:
  int m_width = 0; // width of the register in bytes

  // A pointer to the register.
  // A Union is used so dealing with different register widths
  // isn't as painful
  union
  {
    volatile uint32_t* u32;
    volatile uint16_t* u16;
    volatile uint8_t*  u8;
    volatile void* ptr;
  } m_pReg;
};

#endif // Register_h__