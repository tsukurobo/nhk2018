#include "ros/ros.h"
#include <stdlib.h>
#include <math.h>
#include "std_msgs/String.h"
#include "sensor_msgs/Joy.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
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

//w[]はホイールの制御量 maxは|w[]|の最大数 spは平行移動の速度（0~1） tnは回転の速度 c平行強度 t回転強度 c*2+t<100になるようにする。 m及びtは下の計算式参照。これにより制御の平行移動(m)と回転(t)の比重を変える。
int w[5],max,sp,tn,c,t;
float d=0,length,x,y,m;
//std_msgs::Int8 mv; // 0静止　1動作
std_msgs::Int8MultiArray array;

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy){

//joy_nodeはジョイスティックの値およびボタンのon/offを配列でパブしている。rosrun joy joy_node　してrostopic echo joy　すれば分かる。
//joy->axes[0]が平行移動のジョイスティックの縦方向の値　joy->axes[1]平行移動のジョイスティックの横方向の値 joy->axes[2]が回転のジョイスティックの横方向の値

	
	
	
	//三角関数を用いて並行方向の操作量を求める。
	//d=角度　joy->axes[3]の量により、これに補正をかける。
	//左右のボタンが同時に押されている時
	m=1;
	if(joy->buttons[4]==1&&joy->buttons[5]==0){
		m=5/10;
	}
	//左手ボタンのみが押されている時
	else if(joy->buttons[4]==1&&joy->buttons[5]==1){
		m=3/10;
	}
	else{
	}
	
	d=(atan2(joy->axes[1],joy->axes[0])-joy->axes[3]*0.5)*m;//<<<<<<<<<<<<<<<<<<ここの*0.5の値を変えて回転による平行移動の補正の強さを調整
						      //^^^　この値を調整
	
	
	
	
	
	
	//平行方向のジョイスティックの変位量を求める
	length=sqrt(pow(joy->axes[0],2)+pow(joy->axes[1],2))*m;
	
	c=37.5;
	t=25;
	
	
	//回転方向のジョイスティックの変位量
	tn=joy->axes[3]*t*m;	
	
	
	
	
	
	//縦方向の操作量
	x=cos(d)*length*c;
	//横方向の操作量
	y=sin(d)*length*c;	
	
	w[1]=-x+y-tn;
	w[2]=x+y+tn;
	w[3]=x+y-tn;
	w[4]=-x+y+tn;	

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "controller");
  ros::NodeHandle n;
  ros::Subscriber joy = n.subscribe("joy",1000,joyCallback);
  ros::Publisher pub = n.advertise<std_msgs::Int8MultiArray>("array", 100);
  //ros::Publisher motorR_pub = n.advertise<std_msgs::Int8>("motorR", 1000);
  //ros::Publisher motorL_pub = n.advertise<std_msgs::Int8>("motorL",1000);
  //ros::Publisher motor_pub = n.advertise<std_msgs::Int8MultiArray>("motor", 1000);
  //ros::Publisher motor_pub = n.advertise<tr_controller::motor>("motor",1000);
  //ros::Publisher w1_pub = n.advertise<std_msgs::Int8>("w1",1000);
  //ros::Publisher w2_pub = n.advertise<std_msgs::Int8>("w2",1000);
  //ros::Publisher w3_pub = n.advertise<std_msgs::Int8>("w3",1000);
  //ros::Publisher w4_pub = n.advertise<std_msgs::Int8>("w4",1000);
  //ros::Rate loop_rate(10);
  ros::Rate loop_rate(10);
  int count = 0;
  while (ros::ok())
  {
    array.data.clear();
    for(i=0;i<4;i++){
    	array.data.push_back(w[i+1]);
    }
    pub.publish(array);
    ros::spinOnce();
    loop_rate.sleep();
    ++count;
  }


  return 0;
}
