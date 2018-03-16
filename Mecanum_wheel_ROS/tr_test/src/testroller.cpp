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



//前
//1 1
//1 1
//右
//1-1
//-11
//左
//-11
//1-1
//後ろ
//-1-1
//-1-1
//
std_msgs::Int8 w1,w2,w3,w4;
std_msgs::Int8 mv; // 0静止　1前進　2後退　3左旋回　4右旋回
void joyCallback(const sensor_msgs::Joy::ConstPtr& joy){
  if(joy->axes[1] != 0||joy->axes[0] != 0){
/*    ROS_INFO("RUNNING : forward");
    motorpw_L.data = -(MAXPW * joy->axes[1]);
    motorpw_R.data = -(MAXPW * joy->axes[1]*0.75);
    mv.data = 1;*/

	ROS_INFO("RUNNING(`')");
	w1.data=-50*joy->axes[0]+50*joy->axes[1];
	w2.data=50*joy->axes[0]+50*joy->axes[1];
	w3.data=50*joy->axes[0]+50*joy->axes[1];
	w4.data=-50*joy->axes[0]+50*joy->axes[1];
	mv.data = 1;
  }

  else{
    ROS_INFO("IDLE...");
    w1.data = 0;
    w2.data = 0;
    w3.data = 0;
    w4.data = 0;
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
  ros::Publisher w1_pub = n.advertise<std_msgs::Int8>("w1",1000);
  ros::Publisher w2_pub = n.advertise<std_msgs::Int8>("w2",1000);
  ros::Publisher w3_pub = n.advertise<std_msgs::Int8>("w3",1000);
  ros::Publisher w4_pub = n.advertise<std_msgs::Int8>("w4",1000);
  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok())
  {

    w1_pub.publish(w1);
    w2_pub.publish(w2);
    w3_pub.publish(w3);
    w4_pub.publish(w4);
   
    ros::spinOnce();
    loop_rate.sleep();
    ++count;
  }


  return 0;
}
