#include "LrfPair.h"
#define PARALLAX 100
#define pi 3.1415926536
#define LRF_ERR 5000

LrfPair::LrfPair(int lsh, int rsh):
  sensorL(),sensorR(){
    this->reliability = false;
    this->angle = this->distanceL = this->distanceR = this->distance_avr = 0;

    /*LSHUT = lsh;
    RSHUT = rsh;
    
    pinMode(13,OUTPUT);
    digitalWrite(13,HIGH);

    pinMode(lsh, OUTPUT); 
    digitalWrite(lsh, LOW);
 
    delay(100);
    Wire.begin();
 
    pinMode(lsh, INPUT);
    delay(150);
    this->sensorL.init(true);
    delay(100);
    this->sensorL.setAddress((uint8_t)(4*lsh));
    this->sensorL.setTimeout(500);

    pinMode(rsh, OUTPUT); 
    digitalWrite(rsh, LOW);
 
    delay(100);
    Wire.begin();
 
    pinMode(rsh, INPUT);
    delay(150);
    this->sensorR.init(true);
    delay(100);
    this->sensorR.setAddress((uint8_t)(4*rsh));
    this->sensorR.setTimeout(500);

    #if defined LONG_RANGE //2200くらいまでレンジを広げる
      // lower the return signal rate limit (default is 0.25 MCPS)
      this->sensorL.setSignalRateLimit(0.1);
      // increase laser pulse periods (defaults are 14 and 10 PCLKs)
      this->sensorL.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
      this->sensorL.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

      // lower the return signal rate limit (default is 0.25 MCPS)
      this->sensorR.setSignalRateLimit(0.1);
      // increase laser pulse periods (defaults are 14 and 10 PCLKs)
      this->sensorR.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
      this->sensorR.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
    #endif
 
    this->sensorL.startContinuous();
    this->sensorR.startContinuous();
    digitalWrite(13,LOW);*/
}


void LrfPair::getAngle(){
  float a = this->distanceL - this->distanceR;
  float b = PARALLAX;  // (a/b) = tan 
  this->angle = atan(a/b) * (180/pi);

  if(this->distanceL > LRF_ERR || this->distanceR > LRF_ERR)reliability = false;
  else true;
}

void LrfPair::getDistance(){
  this->distanceL = sensorL.readRangeContinuousMillimeters();
  this->distanceR = sensorR.readRangeContinuousMillimeters();
  this->distance_avr = (distanceL+distanceR)/2;
}
  
