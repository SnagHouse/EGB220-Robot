#include "MotorDriver.h"
#include "Pin.h"

// MotorDriver leftMotor(IOPin(0, 'b'));
// MotorDriver rightMotor(IOPin(7, 'd'));

void setup()
{
  Serial.begin(9600);
  
  // Clear the direction registers
  // Register(DDRD).Clear();
  // Register(DDRB).Clear();

  // leftMotor.Start();
  // rightMotor.Start();
  // leftMotor.SetSpeed(0.5);
  // rightMotor.SetSpeed(0.5);
  Serial.write("setup\n");

  // pinMode(12, OUTPUT);
  // pinMode(13, OUTPUT);
  // digitalWrite(13, HIGH);
  // digitalWrite(12, HIGH);
  
  IOPin redLed(4, 'b');
  IOPin blueLed(5, 'b');
  
  Serial.write("IOPins created\n");

  redLed.Write(true);
  blueLed.Write(true);

  Serial.write("Enabled pins\n");

  delay(2000);

  Serial.write("Turning off red LED\n");
  // digitalWrite(13, LOW);
  redLed.Write(false);
  
  delay(2000);
//  digitalWrite(12, LOW);

  Serial.write("Turning off blue LED\n");
  blueLed.Write(false);
}

void loop()
{
}
