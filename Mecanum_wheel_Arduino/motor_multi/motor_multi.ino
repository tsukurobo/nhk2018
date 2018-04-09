/*
 * rosserial Publisher Example
 *test of Publishing Multi Array
 */
#include <stdio.h>
#include <ros.h>
#include <std_msgs/Int8MultiArray.h>
#include <std_msgs/Int8.h>
#include <stdlib.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>

#include <Wire.h>
#include <stdlib.h>
#include "ti2c.h"
#include "ise_motor_driver.h"


uint8_t addr1 = 0x22;
uint8_t addr2 = 0x20;
uint8_t addr3 = 0x23;
uint8_t addr4 = 0x21;
IseMotorDriver m1 = IseMotorDriver(addr1);
IseMotorDriver m2 = IseMotorDriver(addr2);
IseMotorDriver m3 = IseMotorDriver(addr3);
IseMotorDriver m4 = IseMotorDriver(addr4);



ros::NodeHandle  nh;
std_msgs::Int8MultiArray arrayp;
std_msgs::Int8MultiArray array;
ros::Publisher wparam("wparam", &arrayp);

int pw1,pw2,pw3,pw4;
long enc1,enc2,enc3,enc4;
int w1,w2,w3,w4;
int i=0;
void messageCb(const std_msgs::Int8MultiArray& array)
{
  w1=array.data[0];
  w2=array.data[1];
  w3=array.data[2];
  w4=array.data[3];
}
ros::Subscriber<std_msgs::Int8MultiArray>sub("array",&messageCb);
void setup()
{
  arrayp.data_length=4;
  arrayp.data=(int8_t*)malloc(sizeof(int8_t*)*4);
  Wire.begin();
  //Serial.begin(9600);
  nh.initNode();
  //delay(1000);
  nh.subscribe(sub);
  //delay(1000);
  nh.advertise(wparam);
  //delay(1000);
}

void loop()
{
  
  pw1 = w1;
  pw2 = w2;
  pw3 = w3;
  pw4 = w4;
  
  m1.setSpeed(pw1);
  m2.setSpeed(pw2);
  m3.setSpeed(pw3);
  m4.setSpeed(pw4);
  enc1 = m1.encorder();
  enc2 = m2.encorder();
  enc3 = m3.encorder();
  enc4 = m4.encorder();
  
  arrayp.data[0] = w1;
  arrayp.data[1] = w2;
  arrayp.data[2] = w3;
  arrayp.data[3] = w4;
  wparam.publish( &arrayp );
  nh.spinOnce();
  delay(100);
}
