#include <ros.h>

#include <std_msgs/Int8.h>



#define WHEEL_DIAMETER 40//だいたい

#define STEP 1.8

#define STEPS 200// 360 / STEP (360度の回転に何パルス必要か

#define RPM 30 //分あたりの回転数

#define PULSE_ROLL (WHEEL_DIAMETER * 3.14 * STEP / 360)//１パルスあたり

#define STEPPING_MOTOR_SUM 2

#define DEFAULTPW 80

#define PULSEPIN　//モーター

#define DIRPIN

void pulse(){
  
  static  boolean output = HIGH;
  
  int pretime = micros;//時間の原点をとる
  
  digitalWrite(PULSEPIN,output);//パルスをHIGH
  
  output = !output;
  
  while(){
    int forwtime = micros;
    if(forwtime-pretime==750)break;
  }　　　　　　　　　　　　　　　　　　　//750秒待つ

  digitalWrite(PULSEPIN,output);

  while(){
    int forwtime = micros;
    if(forwtime-pretime==1500)break;
  }

  output = !output;
     
    
    
  
  }



void stpaCallback(const std_msgs::Int16& a){

     int input_pulse = a.data/PULSE_ROLL;

     if(input_plus>=0){
      
      digitalWrite(DIRPIN,HIGH);
      
      for(int i=0,i<=input_plus,i++){
        plus();
  
        }
      
      
      }

     if(input_plus<0){
      input_plus = -input_plus;
      digitalWrite(DIRPIN,LOW);
      
      for(int i=0,i<=input_plus,i++){
        plus();
  
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

  stp_motor_init();

  nh.subscribe(stpa);


}



void loop(){

  delay(10);

  nh.spinOnce();

}
