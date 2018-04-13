#include <Wire.h>
#include <VL53L0X.h>
 
VL53L0X sensorR;
VL53L0X sensorL;
VL53L0X sensorB;
VL53L0X sensorF;
 
#define RSHUT 30
#define LSHUT 34
#define BSHUT 38
#define FSHUT 42
 
void setup() {
pinMode(13,OUTPUT);
digitalWrite(13,HIGH);

pinMode(RSHUT, OUTPUT);
pinMode(LSHUT, OUTPUT);
pinMode(BSHUT, OUTPUT);
pinMode(FSHUT, OUTPUT);

digitalWrite(RSHUT, LOW);
digitalWrite(LSHUT, LOW);
digitalWrite(BSHUT, LOW);
digitalWrite(FSHUT, LOW);
 
delay(100);
Wire.begin();
 
pinMode(LSHUT, INPUT);
delay(150);
sensorL.init(true);
delay(100);
sensorL.setAddress((uint8_t)23);
sensorL.setTimeout(500);
 
pinMode(RSHUT, INPUT);
delay(150);
sensorR.init(true);
delay(100);
sensorR.setAddress((uint8_t)24);
sensorR.setTimeout(500);

pinMode(BSHUT, INPUT);
delay(150);
sensorB.init(true);
delay(100);
sensorB.setAddress((uint8_t)25);
sensorB.setTimeout(500);

pinMode(FSHUT, INPUT);
delay(150);
sensorF.init(true);
delay(100);
sensorF.setAddress((uint8_t)26);
sensorF.setTimeout(500);
 
sensorL.startContinuous();
sensorB.startContinuous();
sensorR.startContinuous();
sensorF.startContinuous();
digitalWrite(13,LOW);
 
Serial.begin(19200);
}
 
void loop(){
Serial.print("R:");
Serial.print(sensorR.readRangeContinuousMillimeters());
Serial.print(" L:");
Serial.print(sensorL.readRangeContinuousMillimeters());
Serial.print(" B:");
Serial.print(sensorB.readRangeContinuousMillimeters());
Serial.print(" F");
Serial.println(sensorF.readRangeContinuousMillimeters());
delay(10);
}
