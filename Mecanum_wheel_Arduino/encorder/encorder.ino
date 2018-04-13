/*
 * rosserial Publisher Example
 *test of Publishing Multi Array
 */
#include <stdio.h>
#include <ros.h>
#include <std_msgs/Int8MultiArray.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Int64MultiArray.h>
#include <std_msgs/Int64.h>
#include <stdlib.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <Wire.h>
#include <stdlib.h>
#include "ti2c.h"
#include "ise_motor_driver.h"

int pw[5]={0,0,0,0,0},w[6]={0,0,0,0,0,0};
long enc[5]={},Enc_a[5]={},Enc_b[5]={},n=180;///18000
double p=10,i=10,d=0.4,e1[5]={},e2[5]={},e3[5]={},e4[5]={};
static int h=0,l=0;
long plus=2000000000;
long minus=-2000000000;
uint8_t addr1 = 0x26;


IseMotorDriver m1 = IseMotorDriver(addr1);



ros::NodeHandle  nh;
std_msgs::Int64MultiArray arrayp;
std_msgs::Int8MultiArray array;
ros::Publisher wparam("wparam", &arrayp);







void messageCb(const std_msgs::Int8MultiArray& array)
{
  for(int i=0;i<5;i++){
     
    w[i+1]=array.data[i];   
  }
  
  //-2,147,483,648 ～ 2,147,483,647
  Enc_b[1]=m1.encorder();
  Enc_a[1] = m1.encorder();
  
  //encorder foward +4-
  if(Enc_b[1]>plus){
    if(Enc_a[1]<=0&&Enc_b[1]>0){
      enc[1]=(4294967295+Enc_a[1]-Enc_b[1])/n;
    }
    else{
      enc[1]=(Enc_a[1]-Enc_b[1])/n;
    }
  }
  
  //encorder back -4+ 
  else if(Enc_b[1]<minus){
    if(Enc_a[1]>=0&&Enc_b[1]<0){
      enc[1]=(-4294967295+Enc_a[1]-Enc_b[1])/n;
    }
    else{
      enc[1]=(Enc_a[1]-Enc_b[1])/n;
    }
  }
  
  else{
    enc[1]=(Enc_a[1]-Enc_b[1])/n;
  }
 
  pw[1]=w[1];
}
ros::Subscriber<std_msgs::Int8MultiArray>sub("array",&messageCb);
void setup()
{
  arrayp.data_length=4;
  arrayp.data=(int64_t*)malloc(sizeof(int64_t*)*4);
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
  m1.setSpeed(pw[1]);
  arrayp.data[0] = enc[1];
  arrayp.data[1] = Enc_a[1];
  arrayp.data[2] = pw[1];
  
  wparam.publish( &arrayp );
  nh.spinOnce();
  delay(10);
}
