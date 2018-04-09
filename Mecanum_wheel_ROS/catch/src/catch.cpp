#include "ros/ros.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "std_msgs/String.h"
#include "sensor_msgs/Joy.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int8MultiArray.h"
#include "std_msgs/Int32MultiArray.h"
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

int w[5],max,sp,tn,c=48,t=25,btn0,btn1,btn2;
float d=0,dt=0,length,x,y,m,joy_side,joy_foward,joy_turn,half=0.5,min=0.3;
//std_msgs::Int8 mv; // 0静止　1動作
std_msgs::Int8MultiArray array;



//@param side:-1~1(left:1 right:-1)  foward:-1~1(foward:1 back:-1) turn:-1~1(clockwise:1 counterclockwise:-1)
//※when Parallel displacement in x degree, side=-cosin(x) foward=sin(x)
void cal(float side,float foward,float turn){
//joy_nodeはジョイスティックの値およびボタンのon/offを配列でパブしている。rosrun joy joy_node　してrostopic echo joy　すれば分かる。
//joy->axes[0]が平行移動のジョイスティックの縦方向の値　joy->axes[1]平行移動のジョイスティックの横方向の値 joy->axes[2]が回転のジョイスティックの横方向の値
	//三角関数を用いて並行方向の操作量を求める。
	//d=角度　joy->axes[3]の量により、これに補正をかける。
	//左右のボタンが同時に押されている時
	m=1;
	if(btn1==1&&btn2==0){
		m=half;
	}
	//左手ボタンのみが押されている時
	else if(btn1==1&&btn2==1){
		m=min;
	}
	else{
	}
	
	//xboxのコントローラは原点に戻らないため、ブレを消去
	if(fabs(turn)>=0.1){
		dt=dt+turn*0.1;
	}
	else{
	}
	if(fabs(foward)<=0.05){
		foward=0;
	}
	else{
	}
	if(fabs(side)<=0.05){
		side=0;
	}
	else{
	}
	
	
	d=(atan2(foward,-(side)));//角度を割り出す。
	
	
	//平行方向のジョイスティックの変位量を求める
	length=sqrt(pow(side,2)+pow(foward,2));
	
	
	//回転方向のジョイスティックの変位量
	tn=turn*t;	
	
	
	
	
	
	//縦方向の操作量
	x=cos(d)*length*c*m;
	//横方向の操作量
	y=sin(d)*length*c*m;
	
	w[1]=x+y-tn;
	w[2]=-x+y+tn;
	w[3]=-x+y-tn;
	w[4]=x+y+tn;
	
	for(int i=1;i<5;i++){
		if(abs(w[i])>100){
			w[i]=100*w[i]/abs(w[i]);
		}
		else{
		}
		/*if(abs(w[i])<3){
			w[i]=0;
		}
		else{
		}*/
	}	
}

//joyの値を変数に入れる
void camCallback(const std_msgs::Int32MultiArray::ConstPtr& image_param){
	static float e1,e2,e3,e4;
	const float p_gain=1,i_gain=0.3,d_gain=0.1;
	int joy_param;
	int cam[12];
    int param = 0;
    // print all the remaining number
    //データの始まりから、終わりまで
    for(std::vector<int>::const_iterator it = image_param->data.begin(); it != image_param->data.end(); ++it)
    {   //cam[i]に*it(array.data[i]の中身)を入れる
        cam[param] = *it;
        param++;
    }
	joy_foward=0;
	joy_turn=0;
	joy_param=cam[3]-320;
	joy_param=joy_param/320;
	/*
	e1=0-joy_side; 
	e2=e2+e1;
	e3=e1-e4;
	e4=e1;
	joy_side=p_gain*e1+i_gain*e2+d_gain*e3;
	if(abs(joy_side)>1){
		joy_side=joy_side/abs(joy_side);
	}
	else{
	}
	*/
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "controller");
  ros::NodeHandle n;
  ros::Subscriber image_param = n.subscribe("image_param",1000,camCallback);
  ros::Publisher pub = n.advertise<std_msgs::Int8MultiArray>("array", 100);
  ros::Rate loop_rate(10);
  int count = 0;
  
  while (ros::ok())
  {
    //操作量を計算する
    
    cal(joy_side,joy_foward,joy_turn);
     ROS_INFO("side=%f",joy_side);
    array.data.clear();
    for(int i=0;i<4;i++){
    	array.data.push_back(w[i+1]);
    }
    pub.publish(array);
    ros::spinOnce();
    loop_rate.sleep();
    ++count;
  }


  return 0;
}
