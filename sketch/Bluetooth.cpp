#include "Bluetooth.h"

static Bluetooth *g_pListening = nullptr;

Bluetooth::Bluetooth(int rx, int tx, int baud)
  : SoftwareSerial(rx, tx)
  , m_connected(false)
{}

bool Bluetooth::write(void const *pData, int len)
{
  uint8_t const * pCursor = (uint8_t const *)pData;
  uint8_t const * pEnd = pCursor + len;
  while (pCursor < pEnd)
    super::write(*pCursor);
}

int Bluetooth::read(void *pBuffer, int maxLength)
{
  super::listen();
  if (super::available())
  {
  }
}

bool Bluetooth::isConnected() const
{
}
