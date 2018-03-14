//23000 pulse / spin (maxon)
#include <Wire.h>
#include <stdlib.h>
#include "ti2c.h"
#include "ise_motor_driver.h"

// main for testing.
uint8_t addr = 0x11;
IseMotorDriver m1 = IseMotorDriver(addr);

int pw = 30;
long enc = 0;

void setup(){
  Wire.begin();
  Serial.begin(115200);
  Serial.println("start");
}

void loop(){
     m1.setSpeed(pw);
     enc = m1.encorder();
     Serial.println(enc);
     delay(1000);
}
