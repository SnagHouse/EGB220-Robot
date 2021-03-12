#include "MotorDriver.h"
#include "Pin.h"

void setup()
{
  Serial.begin(9600);

  DigitalPin redLed(4, 'b');
  DigitalPin blueLed(5, 'b');

  redLed.Write(true);
  blueLed.Write(true);

  delay(2000);
  redLed.Write(false);
  delay(2000);
  blueLed.Write(false);
}

void loop()
{
}
