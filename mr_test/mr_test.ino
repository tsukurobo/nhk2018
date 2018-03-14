#include <ros.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int32.h>
#include <Wire.h>
#include <stdlib.h>
#include "ti2c.h"

Ti2c ti2cl = Ti2c(0x11);
Ti2c ti2cr = Ti2c(0x12);
char buf[100];
int l = 0;
int r = 0;


void motorCallBack(const std_msgs::Int8& num){
}
void motorLCallBack(const std_msgs::Int8& pw){
  l = pw.data;
  char tbuf[100];
  sprintf(tbuf, "%d",l);
  ti2cl.sendStr(tbuf);
}

void motorRCallBack(const std_msgs::Int8& pw){
  r = pw.data;
  char tbuf[100];
  sprintf(tbuf, "%d",r);
  ti2cr.sendStr(tbuf);}


ros::NodeHandle nh;

ros::Subscriber<std_msgs::Int8>mr("ml",motorRCallBack);
ros::Subscriber<std_msgs::Int8>ml("mr",motorLCallBack);



void setup() {
  Wire.begin();
  nh.initNode();
  nh.subscribe(ml);
  nh.subscribe(mr);
  ti2cl.sendStr("0");
  ti2cr.sendStr("0");
  

}

void loop() {
  
  //ti2c.receiveStr(buf);
  //ti2c2.receiveStr(buf);
  nh.spinOnce();
  delay(100);
}
