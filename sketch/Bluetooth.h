#ifndef Bluetooth_h__
#define Bluetooth_h__

#include <SoftwareSerial.h>

class Bluetooth : public SoftwareSerial
{
  typedef SoftwareSerial super;

public:
  /**
   * Construct a serial over bluetooth interface.
   * 
   * @param rx   The pin to use for RX.
   * @param tx   The pin to use for TX.
   * @param baud The baud rate of the serial connection.
   **/ 
  Bluetooth(int rx, int tx, int baud=9600);

  /**
   * Send some data to the connected device. If no device is connected,
   * this function will return false.
   * 
   * @param [in] pData A pointer to the data to send.
   * @param [in] len   The length of the data to send in bytes.
   * @return Returns true if the data was successfully sent.
   **/
  bool write(void const * pData, int len);

  /**
   * Read some data from the connected device.
   * 
   * @param [out] pBuffer   A pointer to buffer to write the data to.
   * @param [in]  maxLength The maximum length to read from the device in bytes.
   *                        This must be <= the length of the buffer
   * @return The number of bytes read into the buffer.
   **/
  int read(void *pBuffer, int maxLength);

  /**
   * Check if the bluetooth module is connected to a device.
   * 
   * @return True if connected, otherwise false.
   **/
  bool isConnected() const;

protected:
  bool m_connected = false;
  uint8_t *m_pRecv;
};

#endif