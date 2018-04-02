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

int pw[5],w[5],n=/**/ ,m[5];
long enc[5]={};
double p=0.95,i=2.4,d=0.4,e1[5]={},e2[5]={},e3[5]={},e4[5]={};

uint8_t addr1 = 0x11;
uint8_t addr2 = 0x12;
uint8_t addr3 = 0x13;
uint8_t addr4 = 0x14;


IseMotorDriver m[1] = IseMotorDriver(addr1);
IseMotorDriver m[2] = IseMotorDriver(addr2);
IseMotorDriver m[3] = IseMotorDriver(addr3);
IseMotorDriver m[4] = IseMotorDriver(addr4);



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
}

void loop()
{
  for(i=0;i<4;i++){
    enc[i] = m[i].encorder();
  }
  //pw=p*e[]+i*e[]+d*e[] 
  //w[]*n
  //enc -100~100
  for(int j=1;j<5;j++){
    e1[j]=w[j]*n-enc[j]; 
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
  for(i=1;i<5;i++){
    m[i].setspeed(pw[i]); 
  }
  
  for(i=0;i<4;i++){
     arrayp.data[i] = w[i+1];
  }
  wparam.publish( &arrayp );
  nh.spinOnce();
  delay(100);
}
