#include <VL53L0X.h>
#include <math.h>
#include <Arduino.h>
#include <Wire.h>
#define LONG_RANGE


class LrfPair{
 public:
  VL53L0X sensorL,sensorR;
  int distanceL,distanceR,distance_avr;
  int angle;
  int LSHUT,RSHUT;
  bool reliability;

  LrfPair(int lsh, int rsh);
  void getAngle();
  void getDistance();
};
