#include <ros.h>

#include <std_msgs/Int8.h>

#include <std_msgs/Int16.h>


#define WHEEL_DIAMETER 25//だいたい

#define STEP 1.8

#define STEPS 200// 360 / STEP (360度の回転に何パルス必要か

#define RPM 30 //分あたりの回転数

#define PULSE_ROLL (WHEEL_DIAMETER * 3.14 * STEP / 360)//１パルスあたり

#define STEPPING_MOTOR_SUM 2

#define DEFAULTPW 80

#define PULSEPIN_A 2
#define PULSEPIN_B 4

#define DIRPIN_A 3
#define DIRPIN_B 5

volatile int input_pulseA = 0;
volatile int input_pulseB = 0;

ros::NodeHandle nh;

void stpaCallback(const std_msgs::Int16& a){
     
     input_pulseA = a.data/PULSE_ROLL;
     
     
}

void stpbCallback(const std_msgs::Int16& b){
     
     input_pulseB = b.data/PULSE_ROLL;
     
     
}

ros::Subscriber<std_msgs::Int16> stpa("stpa",stpaCallback);
ros::Subscriber<std_msgs::Int16> stpb("stpb",stpbCallback);

void pulse_A(){
  
  static  boolean outputA = HIGH;
 
  
  digitalWrite(PULSEPIN_A,outputA);//パルスをHIGH
  
  outputA = !outputA;
  
     
    
    
  
  }

void pulse_B(){
  
  static  boolean outputB = HIGH;
 
  
  digitalWrite(PULSEPIN_B,outputB);//パルスをHIGH
  
  outputB = !outputB;
  
     
    
    
  
  }  







void testA(int b){
     int i,j = 0;
     static int countA = 0;

     if(b>=0){
      Serial.print("+");
      
      digitalWrite(DIRPIN_A,HIGH);
      
      
      pulse_A();

      countA++;
  
     
      
      
      }

     if(b<0){
      Serial.print("-");
      b= -b;
      digitalWrite(DIRPIN_A,LOW);
      
      
      pulse_A();
  
      countA++;
      
      
      }

     if(countA>b*2){
      
      countA = 0;

      while(1){
        nh.spinOnce();

        static int num2A = 0;
        int num1A = input_pulseA;

        if(num1A != num2A ){
          num2A = num1A;

          break;
          
          }
        
        num2A = num1A;
        
        
        }
      
      
     
      }


}


void testB(int b){
     int i,j = 0;
     static int countB = 0;

     if(b>=0){
      Serial.print("+");
      
      digitalWrite(DIRPIN_B,HIGH);
      
      
      pulse_B();

      countB++;
  
     
      
      
      }

     if(b<0){
      Serial.print("-");
      b= -b;
      digitalWrite(DIRPIN_B,LOW);
      
      
      pulse_B();
  
      countB++;
      
      
      }

     if(countB>b*2){
      
      countB = 0;

      while(1){
        nh.spinOnce();

        static int num2B = 0;
        int num1B = input_pulseB;

        if(num1B != num2B ){
          num2B = num1B;

          break;
          
          }
        
        num2B = num1B;
        
        
        }
      
      
     
      }


}














void setup(){
  pinMode(DIRPIN_A,OUTPUT);
  pinMode(PULSEPIN_A,OUTPUT);
  digitalWrite(DIRPIN_A,HIGH);
  digitalWrite(PULSEPIN_A,LOW);
  pinMode(DIRPIN_B,OUTPUT);
  pinMode(PULSEPIN_B,OUTPUT);
  digitalWrite(DIRPIN_B,HIGH);
  digitalWrite(PULSEPIN_B,LOW);
  Serial.begin(9600);

  
  
  nh.initNode();

  nh.subscribe(stpa);
  nh.subscribe(stpb);


}



void loop(){
  Serial.print(1);

  testA(input_pulseA);
  testB(input_pulseB);


  delayMicroseconds(1500);

  nh.spinOnce();

}
