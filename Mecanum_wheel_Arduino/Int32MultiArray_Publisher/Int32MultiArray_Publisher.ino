/*
 * rosserial Publisher Example
 *test of Publishing Multi Array
 */
#include <stdio.h>
#include <ros.h>
#include <std_msgs/Int32MultiArray.h>

ros::NodeHandle  nh;
std_msgs::Int32MultiArray array;
ros::Publisher MultiArray("MultiArray", &array);


void setup()
{
  array.data_length=3;
  array.data=(int32_t*)malloc(sizeof(int32_t*)*3);
  Serial.begin(9600);
  nh.initNode();
  nh.advertise(MultiArray);
}

void loop()
{
   int b=3;
  array.data[1]= b;
  MultiArray.publish( &array );
  nh.spinOnce();
  delay(100);
}
