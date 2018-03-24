#include <Wire.h>
#include "n_line_sensor.h"

#define LINE_SENSOR_ALL_SUM 8 // LINE_SENSOR_AVR_SUM * 2
const uint8_t addr0 = 0x21;
const uint8_t addr1 = 0x22;
NLineSensor lineSensor0 = NLineSensor(addr0, addr1);

int line_sensor_value[LINE_SENSOR_ALL_SUM] = {};

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  Serial.println("start");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  lineSensor0.lineSensor(line_sensor_value);
  int i = 0;
  for (i = 0; i < LINE_SENSOR_ALL_SUM; i++){
    Serial.print(line_sensor_value[i]);
    Serial.print(" ");
  }
  Serial.print("\n");
}
