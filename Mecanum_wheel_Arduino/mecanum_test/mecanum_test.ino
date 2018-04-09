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
#define datalength 4294967295
int pw[5]={0,0,0,0,0},w[6]={0,0,0,0,0,0};
long enc[5]={},Enc[5]={},Enc_a[5]={},Enc_b[5]={},n=14;///18000
double p=0.00000000001,i=0.0,d=0.0,e1[5]={},e2[5]={},e3[5]={},e4[5]={};
//0.95 2.4 0.4
static int h=0,l=0;
long plus=2000000000;
long minus=-2000000000;
int state=0;

uint8_t addr1 = 0x22;
uint8_t addr2 = 0x20;
uint8_t addr3 = 0x23;
uint8_t addr4 = 0x21;


IseMotorDriver m1 = IseMotorDriver(addr1);
IseMotorDriver m2 = IseMotorDriver(addr2);
IseMotorDriver m3 = IseMotorDriver(addr3);
IseMotorDriver m4 = IseMotorDriver(addr4);



ros::NodeHandle  nh;
std_msgs::Int64MultiArray arrayp;
std_msgs::Int8MultiArray array;
ros::Publisher wparam("wparam", &arrayp);




void messageCb(const std_msgs::Int8MultiArray& array)
{
  for(int i=1;i<5;i++){
    w[i]=array.data[i-1];
  }
}
void encorder(){
      
    Enc_b[1] = m1.encorder();
    Enc_b[2] = -m2.encorder();
    Enc_b[3] = m3.encorder();
    Enc_b[4] = -m4.encorder();
    delay(15);
    //-2,147,483,648 ～ 2,147,483,647
    //encorder foward +4-
    Enc_a[1] = m1.encorder();
    Enc_a[2] = -m2.encorder();
    Enc_a[3] = m3.encorder();
    Enc_a[4] = -m4.encorder();
  for(int i=1;i<5;i++){ 

    if(Enc_b[i]>plus){
      if(Enc_a[i]<=0&&Enc_b[i]>0){
        enc[i]=(datalength+Enc_a[i]-Enc_b[i])/n;
      }
      else{
        enc[i]=(Enc_a[i]-Enc_b[i])/n;
      }
    }
      
    //encorder back -4+ 
    else if(Enc_b[i]<minus){
      if(Enc_a[i]>=0&&Enc_b[i]<0){
        enc[i]=(-datalength+Enc_a[i]-Enc_b[i])/n;
      }
      else{
        enc[i]=(Enc_a[i]-Enc_b[i])/n;
      }
    }
    
    else{
      enc[i]=(Enc_a[i]-Enc_b[i])/n;
    }
    enc[i]=enc[i]/3;
  }
}
void cal(){
  for(int i=1;i<5;i++){
    
    e1[i]=w[i]-enc[i]; 
    e2[i]=e2[i]+e1[i];
    e3[i]=e1[i]-e4[i];
    e4[i]=e1[i];
    pw[i]=p*e1[i]+i*e2[i]+d*e3[i];
    if(abs(pw[i])>100){
      pw[i]=100*pw[i]/abs(pw[i]);
    }
    else{
    }
    // pw[i]=w[i];
  }
}
ros::Subscriber<std_msgs::Int8MultiArray>sub("array",&messageCb);
void setup()
{
  arrayp.data_length=8;
  arrayp.data=(int64_t*)malloc(sizeof(int64_t*)*8);
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
  encorder();
  cal();
  m1.setSpeed(pw[1]);
  m2.setSpeed(pw[2]);
  m3.setSpeed(pw[3]);
  m4.setSpeed(pw[4]);
  
  for(int j=0;j<4;j++){
    arrayp.data[j] = enc[j+1];
  }
  for(int j=0;j<4;j++){
    arrayp.data[j+4] = pw[j+1];
  }

  
  wparam.publish( &arrayp );
  nh.spinOnce();
  delay(10);
}
