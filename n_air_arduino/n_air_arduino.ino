//23000 pulse / spin (maxon)
#include <Wire.h>
#include <stdlib.h>
#include "ti2c.h"
#include "n_air.h"

// main for testing.
uint8_t addr = 0x11;
NAir air = NAir(addr);



void setup(){
  Wire.begin();
  Serial.begin(115200);
  Serial.println("start");

  air.init();
}

void loop(){
  air.toggle(3);
     
  delay(1000);
}
