#include <ros.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int32.h>
#include <Wire.h>
#include <stdlib.h>
#include "ti2c.h"
#include "ise_motor_driver.h"

unit8_t addrl = 0x11;
unit8_t addrr = 0x12;

IseMotorDriver ml = IseMotorDriver(addrl);
IseMotorDriver mr = IseMotorDriver(addrr);
int l = 0;
int r = 0;

void motorLCallBack(const std_msgs::Int8& pw){
  l = pw.data;
  ml.setSpeed(l);
}
void motorRCallBack(const std_msgs::Int8& pw){
  r = pw.data;
  mr.setSpeed(r);
}

ros::NodeHandle nh;
ros::Subscriber<std_msgs::Int8>mr("mr",motorRCallBack);
ros::Subscriber<std_msgs::Int8>ml("ml",motorLCallBack);


void setup(){
  Wire.begin();
  nh.initNode();
  nh.subscribe(ml);
  nh.subscribe(mr);
}

void loop() {
  nh.spinOnce();
  delay(100);
}
