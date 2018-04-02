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
#include <FlexiTimer2.h>
#include <Wire.h>
#include <stdlib.h>
#include "ti2c.h"
#include "ise_motor_driver.h"

int pw[5],w[5];
long enc[5]={},Enc_a[5]={},Enc_b[5]={},n=1;
double p=0.95,i=2.4,d=0.4,e1[5]={},e2[5]={},e3[5]={},e4[5]={};

uint8_t addr1 = 0x11;
uint8_t addr2 = 0x12;
uint8_t addr3 = 0x13;
uint8_t addr4 = 0x14;


IseMotorDriver m1 = IseMotorDriver(addr1);
IseMotorDriver m2 = IseMotorDriver(addr2);
IseMotorDriver m3 = IseMotorDriver(addr3);
IseMotorDriver m4 = IseMotorDriver(addr4);



ros::NodeHandle  nh;
std_msgs::Int8MultiArray arrayp;
std_msgs::Int8MultiArray array;
ros::Publisher wparam("wparam", &arrayp);







void messageCb(const std_msgs::Int8MultiArray& array)
{
  for(int i=0;i<4;i++){
    w[i+1]=array.data[i];    
  }
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
  FlexiTimer2::set(1, 1.0/1000.0, encorder);
  FlexiTimer2::start();
}

void encorder(){//Enc_a after  Enc_b before
  Enc_a[1] = m1.encorder();
  enc[1]=(Enc_a[1]-Enc_b[1])/n;
  Enc_b[1]=Enc_a[1];
  
}


void loop()
{
  
  m1.setSpeed(pw[1]);
  m1.setSpeed(pw[2]);
  m1.setSpeed(pw[3]);
  m1.setSpeed(pw[4]);
  arrayp.data[1] = enc[1];
  
  wparam.publish( &arrayp );
  nh.spinOnce();
  delay(100);
}
