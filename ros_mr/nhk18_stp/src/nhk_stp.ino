#include <ros.h>
#include <std_msgs/Int8.h>

#define WHEEL_DIAMETER 100 //不明
#define STEP 2.0 //不明
#define PW 100
#define PULSE_FORWARD (WHEEL_DIAMETER * 3.14 * STEP / 360)

void stpaCallback(const std_msgs::Int8& a){
}
void sptbCallback(const std_msgs::Int8& b){
}

ros::NodeHandle nh;
ros::Subscriber<std_msgs::Int8> stpa("stpa",stpaCallback);
ros::Subscriber<std_msgs::Int8> stpb("stpb",stpbCallback);

static const int motor_cw[2] = {3, 4};
static const int motor_ccw[2] = {2, 5};

void setup(){
  nh.initNode();
  nh.subscribe(stpa);
  nh.subscribe(stpb);
}

void loop(){
  
}