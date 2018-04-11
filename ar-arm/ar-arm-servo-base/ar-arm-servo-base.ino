#include <ros.h>

#include <std_msgs/Int8.h>

#include <std_msgs/Int16.h>

#include <Servo.h>
#define arm1_PIN 13
#define arm2_PIN 12
#define arm1_open 180
#define arm1_close 100

#define arm2_open 180
#define arm2_close 100

 

Servo arm1;
Servo arm2;

volatile int triger = 0;

ros::NodeHandle nh;

void No_4(){
  
  arm1.write(arm1_close);
  arm2.write(arm2_close);
  
  }

void No_7(){
  
  arm1.write(arm1_open);
  arm2.write(arm2_open);
  
  }




void Callback(const std_msgs::Int16& a){
     
      triger = a.data;
     
     
}

ros::Subscriber<std_msgs::Int16> num("num",Callback);


void setup() {

  pinMode(arm1_PIN,OUTPUT);
  pinMode(arm2_PIN,OUTPUT);
  arm1.attach(arm1_PIN);
  arm2.attach(arm2_PIN);
  arm1.write(arm1_open);
  arm2.write(arm2_open);

  nh.initNode();

  nh.subscribe(num);
  // put your setup code here, to run once:

}

void loop() {

  if(triger == 4){

    No_4();
    
    }

   if(triger == 7){

    No_7();
    
    }




  nh.spinOnce();

  // put your main code here, to run repeatedly:

}
