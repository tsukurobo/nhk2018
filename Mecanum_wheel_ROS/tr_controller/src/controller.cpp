#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Joy.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Int8.h"
//#include "tr_controller/motor.h"
#include <sstream>
#define THRESHOLD 0.2
#define DEFAULTPW 100
#define MAXPW 150

std_msgs::Int8 motorpw_L,motorpw_R;
std_msgs::Int8 mv; // 0静止　1前進　2後退　3左旋回　4右旋回
void joyCallback(const sensor_msgs::Joy::ConstPtr& joy){
  if(joy->axes[1] >= THRESHOLD){
    ROS_INFO("RUNNING : forward");
    motorpw_L.data = -(MAXPW * joy->axes[1]);
    motorpw_R.data = -(MAXPW * joy->axes[1]*0.75);
    mv.data = 1;
  }
  else if(joy->axes[1] <= -THRESHOLD){
    ROS_INFO("RUNNING : backward");
    motorpw_L.data = -(MAXPW * joy->axes[1]);
    motorpw_R.data = -(MAXPW * joy->axes[1]);
    mv.data = 2;
  }
  else if(joy->axes[0] >= THRESHOLD){
    ROS_INFO("RUNNING : left");
    motorpw_L.data = -(MAXPW * joy->axes[0]);
    motorpw_R.data = (MAXPW * joy->axes[0]);
    mv.data = 3;
  }
  else if(joy->axes[0] <= -THRESHOLD){
    ROS_INFO("RUNNING : right");
    motorpw_L.data = -(MAXPW * joy->axes[0]);
    motorpw_R.data = (MAXPW * joy->axes[0]);
    mv.data = 4;
  }  
  else{
    ROS_INFO("IDLE...");
    motorpw_L.data = 0;
    motorpw_R.data = 0;
    mv.data = 0;
  }
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "controller");
  ros::NodeHandle n;
  ros::Subscriber joy = n.subscribe("joy",1000,joyCallback);
  //ros::Publisher motorR_pub = n.advertise<std_msgs::Int8>("motorR", 1000);
  //ros::Publisher motorL_pub = n.advertise<std_msgs::Int8>("motorL",1000);
  //ros::Publisher motor_pub = n.advertise<std_msgs::Int8MultiArray>("motor", 1000);
  //ros::Publisher motor_pub = n.advertise<tr_controller::motor>("motor",1000);
  ros::Publisher l_pub = n.advertise<std_msgs::Int8>("ml",1000);
  ros::Publisher r_pub = n.advertise<std_msgs::Int8>("mr",1000);
  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok())
  {

    l_pub.publish(motorpw_L);
    r_pub.publish(motorpw_R);
   
    ros::spinOnce();
    loop_rate.sleep();
    ++count;
  }


  return 0;
}
