#include <ros.h>

#include <std_msgs/Int8.h>

#include <std_msgs/Int16.h>

#include "ti2c.h"
#include "ise_motor_driver.h"
#include <Wire.h>
#define DC2_INIT 0
#define DC2_PASS 530

#define DC1_INIT 0
#define DC1_PICK1 -1700
#define DC1_PICK2 -6000
#define DC1_PASS  -3500

uint8_t addr1 = 0x24;
uint8_t addr2 = 0x25;

volatile int triger = 0;

IseMotorDriver m1 = IseMotorDriver(addr1);
IseMotorDriver m2 = IseMotorDriver(addr2);

ros::NodeHandle nh;

void DC_Control1(long n){

  long val = m1.encorder();

  long dif = val-n;

  int speednum = 0;

  if(dif>100 ){

    speednum =50;

    
    }

  if(dif <-100){

    speednum = -50;
    
    }
  
  m1.setSpeed(speednum);
  
  
  
  }



void DC_Control2(long k){

  long val2 = m2.encorder();

  long dif2 = val2-k;

  int speednum2 = 0;

  if(dif2>10 ){

    speednum2 =50;

    
    }

  if(dif2 <-10){

    speednum2 = -50;
    
    }
  
  m2.setSpeed(speednum2);
  
  
  
  }

void No_2(){

  DC_Control2(DC2_INIT);
  
  }

void No_3(){
  
  DC_Control1(DC1_PICK1);
  
  }

void No_3_B(){
  
  DC_Control1(DC1_PICK2);
  
  }

void No_5(){

  DC_Control1(DC1_INIT);
  
  
  }

void No_6(){
  
  DC_Control1(DC1_PASS);
  DC_Control2(DC2_PASS);
  
  }

void No_8(){
  
  DC_Control1(DC1_INIT);
  DC_Control2(DC2_INIT);
  
  }





void Callback(const std_msgs::Int16& a){
     
      triger = a.data;
     
     
}

ros::Subscriber<std_msgs::Int16> num("num",Callback);


void setup() {
  Wire.begin();
  delay(1000);

  nh.initNode();

  nh.subscribe(num);
  // put your setup code here, to run once:

}

void loop() {

  if(triger == 2){

    No_2();
    
    }

   if(triger == 3){

    No_3();
    
    }

    if(triger == 13){

    No_3_B();
    
    }

   if(triger == 5){

    No_5();
    
    }


   if(triger == 6){

    No_6();
    
    }

   if(triger == 8){

    No_8();
    
    }



  
  
  nh.spinOnce();

  // put your main code here, to run repeatedly:

}
