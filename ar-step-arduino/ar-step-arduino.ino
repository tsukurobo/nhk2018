#include <ros.h>

#include <std_msgs/Int8.h>

#include <std_msgs/Int16.h>


#define WHEEL_DIAMETER 40//だいたい

#define STEP 1.8

#define STEPS 200// 360 / STEP (360度の回転に何パルス必要か

#define RPM 30 //分あたりの回転数

#define PULSE_ROLL (WHEEL_DIAMETER * 3.14 * STEP / 360)//１パルスあたり

#define STEPPING_MOTOR_SUM 2

#define DEFAULTPW 80

#define PULSEPIN 1

#define DIRPIN 2

void pulse(){
  
  static  boolean output = HIGH;
  
  int pretime = micros;//時間の原点をとる
  
  digitalWrite(PULSEPIN,output);//パルスをHIGH
  
  output = !output;
  
  while(1){
    int forwtime = micros;
    if(forwtime-pretime==750)break;
  }

  digitalWrite(PULSEPIN,output);

  while(1){
    int forwtime = micros;
    if(forwtime-pretime==1500)break;
  }

  output = !output;
     
    
    
  
  }



void stpaCallback(const std_msgs::Int16& a){
     int i,j = 0;
     int input_pulse = a.data/PULSE_ROLL;

     if(input_pulse>=0){
      
      digitalWrite(DIRPIN,HIGH);
      
      for( i=0;i<=input_pulse;i++){
        pulse();
  
        }
      
      
      }

     if(input_pulse<0){
      input_pulse = -input_pulse;
      digitalWrite(DIRPIN,LOW);
      
      for( j=0;j<=input_pulse;j++){
        pulse();
  
        }
      
      
      }


}




ros::NodeHandle nh;

ros::Subscriber<std_msgs::Int16> stpa("stpa",stpaCallback);




void setup(){
  pinMode(DIRPIN,OUTPUT);
  pinMode(PULSEPIN,OUTPUT);
  digitalWrite(DIRPIN,HIGH);
  digitalWrite(PULSEPIN,LOW);
  
  nh.initNode();

  nh.subscribe(stpa);


}



void loop(){

  delay(10);

  nh.spinOnce();

}
