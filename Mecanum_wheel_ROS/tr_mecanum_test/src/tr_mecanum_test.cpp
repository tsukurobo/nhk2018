#include "ros/ros.h"
#include <stdlib.h>
#include <math.h>
#include "std_msgs/String.h"
#include "sensor_msgs/Joy.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int8MultiArray.h"
#include <sstream>
#define THRESHOLD 0.2
#define DEFAULTPW 100
#define MAXPW 150



//このプログラムではxboxのコントローラをつかいました。よってjoyの配列の値が少々異なります。
//ジョイスティックの可動範囲が円になっているため、spの値を中心からの移動量にしました。



//回転による平行移動の補正のパラメータ調整が済んでいません。暇な人は調整お願いします。




//コメントアウトはmultipi及びmultisを読んでいるものとして書く。
//下記は平行移動の際のホイールの回転方向（１は順回転−１は逆回転）

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

//w[]はホイールの制御量 maxは|w[]|の最大数 spは平行移動の速度（0~1） tnは回転の速度 c平行強度 t回転強度 btn1&btn2ボタンの値  m及びtは下の計算式参照。これにより制御の平行移動(m)と回転(t)の比重を変える。
int w[5],max,sp,tn,c=48,t=25,btn0,btn1,btn2;
//d角度 dt角度に加算する角度 lengthジョイスティックの傾き x横方向の操作量 y縦方向の操作量 m係数 side左右のジョイスティックの値 foward前後のジョイスティックの値 turn回転のジョイスティックの値 
float d=0,dt=0,length,x,y,m,side,foward,turn,half=0.5,min=0.3;
//std_msgs::Int8 mv; // 0静止　1動作
std_msgs::Int8MultiArray array;
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
	if(btn0==1){
		dt=0;
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
	for(int j=1;j<5;j++){
		w[j]=w[j];
	}
	
	for(int i=1;i<5;i++){
		if(w[i]>100){
			w[i]=100;
		}
		else{
		}
	}	
}

//joyの値を変数に入れる
void joyCallback(const sensor_msgs::Joy::ConstPtr& joy){
    
	//joy->axesおよびjoy->bottonsの値を変数に入れる
	side = joy->axes[0];
	foward=joy->axes[1];
	turn=joy->axes[3];
	btn0=joy->buttons[0];
	btn1=joy->buttons[4];
	btn2=joy->buttons[5];

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "controller");
  ros::NodeHandle n;
  ros::Subscriber joy = n.subscribe("joy",1000,joyCallback);
  ros::Publisher pub = n.advertise<std_msgs::Int8MultiArray>("array", 100);
  ros::Rate loop_rate(10);
  int count = 0;
  
  while (ros::ok())
  {
    //操作量を計算する
    cal(side,foward,turn);
    ROS_INFO("d=%f",d);
    
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
