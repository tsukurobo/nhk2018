#ifndef N_LINE_SENSOR_h
#define N_LINE_SENSOR_h
#include "ti2c.h"
#include "Arduino.h"

class NLineSensor {
  public:
      NLineSensor(uint8_t i2cAddr0, uint8_t i2cAddr1);
      void lineSensor(int arr[]);
  private:
      uint8_t addr0;
      uint8_t addr1;
      Ti2c ti2c0;
      Ti2c ti2c1;
};
#endif
