#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "std_msgs/Int8"

#define STOP 0
#define FORWARD 1
#define BACK 2
#define TURNL 3
#define TURNR 4

#define THRESHOLD 0.7
#define TOPPOWER 100
#define LRGAP 5 //壁伝い走行用の回転差

int status = STOP;//状態
int status_buf = STOP;//直前の状態
int r_ispushed,l_ispushed;
int sw = 0;
int span_ms = 100;//速度？積算のタイムスパン
int cnt = 0;//タイムスパン用カウンタ

float delta = 0.2;//PID制御用の係数 pgain
int motorpw_l = 0;
int motorpw_r = 0;

void set_motor_speed(int motor_pw,int target_pw){//PID制御
  int gap = delta * (target_pw - motor_pw);
  if(gap == 0)gap = 1;
  
  if(motor_pw < target_pw)motor_pw += gap;
  else if(motor_pw > target_pw)motor_pw -= gap;
}


void set_motor_status(){
  if(cnt == span_ms){
    if(status == FORWARD){
      set_motor_speed(motorpw_l,TOPPOWER);
      set_motor_speed(motorpw_r,TOPPOWER);
    }else if(status == BACK){
      set_motor_speed(motorpw_l,-TOPPOWER);
      set_motor_speed(motorpw_r,-TOPPOWER);
    }else if(status == STOP){
      set_motor_speed(motorpw_l,0);
      set_motor_speed(motorpw_r,0);
    }
    cnt = 0;
  }
  cnt++;
}

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy){
  if(joy->axes[1] >= THRESHOLD)status = FORWARD;
  else if(joy->axes[1] <= -THRESHOLD)status = BACK;
  else status = STOP;

  if(joy->buttons[5]){//Rボタン（右寄り走行）
    r_ispushed = 1;
  }else r_ispushed = 0;
  if(joy -> buttons[4]){//Lボタン（左寄り走行）
    l_ispushed = 1;
  }else l_ispushed = 0;

  if(status != status_buf){
    status_buf = status;//状態が切り替わった直後の初期化処理
    cnt = 0;
  }
}


int main (int argc, char **argv){
  r_ispushed = 0;
  l_ispushed = 0;
  ros::init(argc,argv,"nhk18_controller");
  ros::NodeHandle nh;
  ros::Subscriber joy = nh.subscribe("joy", 1000, joyCallback);
  ros::Publisher mr_pub = nh.advertise<std_msgs::Int8>("mr",1000);
  ros::Publisher ml_pub = nh.advertise<std_msgs::Int8>("ml",1000);
  ros::Publisher stpa_pub = nh.advertise<std_msgs::Int8>("stpa",1000);
  ros::Publisher stpb_pub = nh.advertise<std_msgs::Int8>("stpb",1000);
  ros::Rate loop_rate(10);

  while(ros::ok()){
    set_motor_status();
    
    if(l_ispushed == 0 && r_ispushed == 1){
      mr_pub.publish(motorpw_r + LRGAP);
      ml_pub.publish(motorpw_l);
    }else if(l_ispushed == 1 && r_ispushed == 0){
      mr_pub.publish(motorpw_r);
      ml_pub.publish(motorpw_l + LRGAP);
    }else{
      mr_pub.publish(motorpw_r);
      ml_pub.publish(motorpw_l);
    }

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
