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
  Enc_a[2] = m2.encorder();
  Enc_a[3] = m3.encorder();
  Enc_a[4] = m4.encorder();
  
  for(int i=1;i<5;i++){
    enc[i]=(Enc_a[i]-Enc_b[i])/n;
  }
  for(int i=1;i<5;i++){
    Enc_b[i]=Enc_a[i];
  }
}


void loop()
{
  //pw=p*e[]+i*e[]+d*e[] 
  //w[]
  //enc -100~100
  for(int j=1;j<5;j++){
    e1[j]=w[j]-enc[j]; 
    e2[j]=e2[j]+e1[j];
    e3[j]=e1[j]-e4[j];
    e4[j]=e1[j];
    pw[j]=p*e1[j]+i*e2[j]+d*e3[j];
    if(abs(pw[j])>100){
      pw[j]=100*pw[j]/abs(pw[j]);
    }
    else{
    }
  }
  m1.setSpeed(pw[1]);
  m2.setSpeed(pw[2]);
  m3.setSpeed(pw[3]);
  m4.setSpeed(pw[4]);
  
  for(int i=0;i<4;i++){
    arrayp.data[i] = w[i+1];
  }
  
  wparam.publish( &arrayp );
  nh.spinOnce();
  delay(100);
}
