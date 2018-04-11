#include "ti2c.h"
#include "ise_motor_driver.h"
#include <Wire.h>

#define FIN 5000
#define INIT 5304
uint8_t addr1 = 0x24;


IseMotorDriver m1 = IseMotorDriver(addr1);



void setup() {

  Serial.begin(9600);
  // put your setup code here, to run once:
  Serial.println("setup");
  Wire.begin();
  delay(1000);
}

void loop() {

  int s = 1;


  long val = m1.encorder();

  long dif = val-FIN;

  int speednum = 0;

  if(dif>100 ){

    speednum =100;

    
    }

  if(dif <-500){

    speednum = -100;
    
    }

  


  





  
  m1.setSpeed(speednum);

  

  
  Serial.println(val);
 
  // put your main code here, to run repeatedly:

}
