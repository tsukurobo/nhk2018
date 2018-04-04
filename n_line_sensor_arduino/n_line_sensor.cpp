#include "Arduino.h"
#include "n_line_sensor.h"
#include "ti2c.h"
#define LINE_SENSOR_AVR_SUM 4
#define LINE_SENSOR_ALL_SUM 8 // LINE_SENSOR_AVR_SUM * 2

NLineSensor::NLineSensor(uint8_t i2cAddr0, uint8_t i2cAddr1) :
  ti2c0(i2cAddr0), ti2c1(i2cAddr1){
  // @param uint8_t i2cAddr: i2c addr of motor driver
  //initializer
  this->addr0 = i2cAddr0;
  this->addr1 = i2cAddr1;
}

void NLineSensor::lineSensor(int arr[]){
  //@param arr: result of line-sensor-values
  char buf[100];
  char str[10]; // "0"~"1023"
  int i = 0, j = 0, k = 0;
  int buf_index = 0;
  // sprintf(buf, "%d", power);
  // this->ti2c.sendStr(buf);

  buf_index = 0;
  this->ti2c0.receiveStr(buf);
  // "value0,value1,value2,value3" -> [value0, value1, value2, value3]
  for (i = 0; i < LINE_SENSOR_AVR_SUM; i++) {
    k = 0;
    while(true) {
      char c = buf[buf_index++];
      if (c == ',' || c == '\0') {
        str[k++] = '\0';
        break;
      } else {
        // get value
        str[k++] = c;
      }
    }
    // string to int
    arr[i] = atoi(str);
  }

  // "value4,value5,value6,value7 -> [value0, value1, value2, value3, value4, value5, value6, value7]"
  buf_index = 0;
  this->ti2c1.receiveStr(buf);
  // "value0,value1,value2,value3" -> [value0, value1, value2, value3]
  for (i = LINE_SENSOR_AVR_SUM; i < LINE_SENSOR_AVR_SUM * 2; i++) {
    k = 0;
    while(true) {
      char c = buf[buf_index++];
      if (c == ',' || c == '\0') {
        str[k++] = '\0';
        break;
      } else {
        // get value
        str[k++] = c;
      }
    }
    // string to int
    arr[i] = atoi(str);
  }

}
