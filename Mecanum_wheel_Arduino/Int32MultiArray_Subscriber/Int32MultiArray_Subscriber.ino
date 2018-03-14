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

int w1,w2,w3,w4;

ros::NodeHandle  nh;
std_msgs::Int32MultiArray array;

void messageCb(const std_msgs::Int32MultiArray& array)
{
  w1=array.data[0];
  w2=array.data[1];
  w3=array.data[2];
  w4=array.data[3];
 
}


ros::Subscriber<std_msgs::Int32MultiArray>sub("Multiarray_sub",&messageCb);

void setup()
{
  //array.data_length=4;
  //array.data=(int32_t*)malloc(sizeof(int32_t*)*4);
  Serial.begin(9600);
  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{
  nh.spinOnce();
  delay(100);
}
