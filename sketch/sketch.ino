// #include "MotorDriver.h"
#include "Commands.h"
#include "avr/io.h"

// MotorDriver leftMotor('b', 0);
// MotorDriver rightMotor('d', 1);

Commands commands;

int someParameter = 0;

void doSomething()
{
  char temp[50];
  sprintf(temp,"someParameter is %d\n", someParameter);
  Serial.write(temp);
}

void setup() {
  Serial.begin(9600);
  
  commands.addVariable("someParameter", &someParameter);
  commands.addCommand("someCommand", doSomething);

  commands.set<int>("someParameter", 1);
  commands.call("someCommand");
  commands.set<int>("someParameter", 10);
  commands.call("someCommand");
}

void loop() {

}
