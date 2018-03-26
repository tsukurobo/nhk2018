//23000 pulse / spin (maxon)
#include <Wire.h>
#include <stdlib.h>
#include "ti2c.h"
#include "n_air.h"

// main for testing.
uint8_t addr = 0x14;
NAir air = NAir(addr);


void setup(){
  Wire.begin();
  Serial.begin(115200);
  Serial.println("start");

  air.init();
}

int n;

void loop(){
  
  if (Serial.available() > 0) { // 受信したデータが存在する
    n = Serial.read(); // 受信データを読み込む

     n = n - '0';

    Serial.print("I received: "); // 受信データを送りかえす
    Serial.println(n, DEC);


   
    air.toggle(n);
  }
}
