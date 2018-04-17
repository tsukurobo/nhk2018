#include "ros/ros.h"
#include <stdlib.h>
#include <math.h>
#include "sensor_msgs/Joy.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int8MultiArray.h"
#include "std_msgs/Int32MultiArray.h"
#include <sstream>
#include "conf.h"




// pub/sub

ros::Subscriber image_sub1;
ros::Subscriber image_sub2;
ros::Publisher mecanum_pub;
std_msgs::Int8MultiArray array;

float image_param1[4];
float image_param2[4];
int w[5];

void calcMotorPower(float radian,float length,int lrf,int line) {
  /**
   * @param degree: -180~180
   * @param length: 0~1
   * @param lrf: 0 or 1 0:flont 1:left
   *@oparam line 0 or 1 1:use
   */
	float x,y,tn,dt,c=15,t=5,maxw,gain_wheel2=1.5,gain_wheel4=1.1;
	//c<100-t
	
	
	//radian=degree*M_PI/180.0;
	
	x=cos(radian)*length*c;
	
	y=sin(radian)*length*c;
	
	w[1]=x+y-tn;
	w[2]=-x+y+tn;
	w[3]=-x+y-tn;
	w[4]=x+y+tn;
	
	w[2]=w[2]*gain_wheel2;
	w[4]=w[4]*gain_wheel4;
	
	for(int i=1;i<5;i++){
		if(abs(w[i])>100){
			w[i]=100*w[i]/abs(w[i]);
		}
		else{
		}
	}
	array.data.clear();
	array.data.push_back(w[1]);
	array.data.push_back(w[2]);
	array.data.push_back(w[3]);
	array.data.push_back(w[4]);
	mecanum_pub.publish(array);
} 

void imageCallback1(const std_msgs::Int32MultiArray::ConstPtr& imagepm)
{
    int in = 0;
    for(std::vector<int32_t>::const_iterator it = imagepm->data.begin(); it != imagepm->data.end(); ++it)
    { 
        image_param1[in] = *it;
        in++;
    }
}
void imageCallback2(const std_msgs::Int32MultiArray::ConstPtr& imagepm)
{
    int in2 = 0;
    for(std::vector<int32_t>::const_iterator it = imagepm->data.begin(); it != imagepm->data.end(); ++it)
    { 
        image_param2[in2] = *it;
        in2++;
    }
}

void pickcock(){
	float p_gain=0.003,i_gain=0,d_gain=0,p_gain2=0.004,i_gain2=0,d_gain2=0,yoko,tate,sq;
	double e1,e3,power,e21,e23,range;
	static double e2,e4,e22,e24;

	e21=image_param2[3]-320;
	e22=e22+e21;
	e23=e21-e24;
	e24=e21;

	tate=p_gain2*e21+i_gain2*e22+d_gain2*e23;

	e1=image_param1[3]-320;
	e2=e2+e1;
	e3=e1-e4;
	e4=e1;

	yoko=p_gain*e1+i_gain*e2+d_gain*e3;
	
	range=atan2(tate,yoko);
	
	power=sqrt(pow(tate,2)+pow(yoko,2));
	
	
	if(image_param1[2]==1&&image_param2[2]==1){
		calcMotorPower(range,power,DO_NOT_USE_LRF,DO_NOT_USE_LINE);
	}
	else if(image_param1[2]==1&&image_param2[2]==0){;
		calcMotorPower(0,power,DO_NOT_USE_LRF,DO_NOT_USE_LINE);
	}
	else if(image_param1[2]==0&&image_param2[2]==1){
		calcMotorPower(90,power,DO_NOT_USE_LRF,DO_NOT_USE_LINE);
	}
	else{
		calcMotorPower(0,0,DO_NOT_USE_LRF,DO_NOT_USE_LINE);
	}
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "mecanum_auto");
  ros::NodeHandle n;
  mecanum_pub = n.advertise<std_msgs::Int8MultiArray>("mecanum_motors", 100);
  image_sub1 = n.subscribe("image_param",100,imageCallback1);
  image_sub2 = n.subscribe("image_param2",100,imageCallback2);
  ros::Rate loop_rate(10);
  
  while (ros::ok())
    {
      pickcock();
      ros::spinOnce();
      loop_rate.sleep();
    }

  return 0;
}
