/*
 * rosserial Publisher Example
 *test of Publishing Multi Array
 */
#include <stdio.h>
#include <ros.h>
#include <std_msgs/Int32MultiArray.h>

#include <stdlib.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>

ros::NodeHandle  nh;
std_msgs::Int32MultiArray arrayp;
std_msgs::Int32MultiArray array;
ros::Publisher wparam("wparam", &arrayp);

int w1,w2,w3,w4;
void messageCb(const std_msgs::Int32MultiArray& array)
{
  w1=array.data[0];
  w2=array.data[1];
  w3=array.data[2];
  w4=array.data[3];
 
}
ros::Subscriber<std_msgs::Int32MultiArray>sub("array",&messageCb);
void setup()
{
  arrayp.data_length=4;
  arrayp.data=(int32_t*)malloc(sizeof(int32_t*)*4);
  //Serial.begin(9600);
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(wparam);
}

void loop()
{
   arrayp.data[0] = w1;
   arrayp.data[1] = w2;
   arrayp.data[2] = w3;
   arrayp.data[3] = w4;
  wparam.publish( &arrayp );
  nh.spinOnce();
  delay(100);
}
