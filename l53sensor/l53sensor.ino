#include <math.h>
#include <Wire.h>
#include <VL53L0X.h>
#define PARALLAX 100 //２つのセンサー視差。mm。
#define LONG_RANGE
#define pi 3.14159265358979
 
VL53L0X sensorR;
VL53L0X sensorL;
int L_distance,R_distance;
 
#define RSHUT 6
#define LSHUT 9

double getAngle(int l, int r){
  double a = l-r;
  double b = PARALLAX; // (a/b) = tan 
  double angle = atan(a/b) * (180/pi);
  return angle;
}
 
void setup() {
pinMode(13,OUTPUT);
digitalWrite(13,HIGH);
pinMode(RSHUT, OUTPUT);
pinMode(LSHUT, OUTPUT);
digitalWrite(RSHUT, LOW);
digitalWrite(LSHUT, LOW);
#if defined LONG_RANGE // ⇐なぜかロングレンジにならない
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensorR.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensorR.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensorR.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
    // lower the return signal rate limit (default is 0.25 MCPS)
  sensorL.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensorL.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensorL.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif
 
delay(100);
Wire.begin();
 
pinMode(LSHUT, INPUT);
delay(150);
sensorL.init(true);
delay(100);
sensorL.setAddress((uint8_t)24);
sensorL.setTimeout(500);

 
pinMode(RSHUT, INPUT);
delay(150);
sensorR.init(true);
 
delay(100);
sensorR.setAddress((uint8_t)23);
sensorR.setTimeout(500);
 
sensorL.startContinuous();
sensorR.startContinuous();
digitalWrite(13,LOW);
 
Serial.begin(19200);
}
 
void loop(){
  R_distance = sensorR.readRangeContinuousMillimeters();
  L_distance = sensorL.readRangeContinuousMillimeters();
Serial.print("R:");
Serial.print(R_distance);
Serial.print(" L:");
Serial.print(L_distance);
Serial.print("  Angle");
Serial.println(getAngle(L_distance,R_distance));
delay(100);
}
