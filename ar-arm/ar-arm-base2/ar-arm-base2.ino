#include <ros.h>

#include <std_msgs/Int8.h>

#include <std_msgs/Int16.h>

#include "ti2c.h"
#include "ise_motor_driver.h"
#include <Wire.h>

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

  if(dif>200 ){

    speednum =100;

    
    }

  if(dif <-200){

    speednum = -100;
    
    }
  
  m1.setSpeed(speednum);
  
  
  
  }



void DC_Control2(long k){

  long val = m2.encorder();

  long dif = val-k;

  int speednum = 0;

  if(dif>10 ){

    speednum =100;

    
    }

  if(dif <-10){

    speednum = -100;
    
    }
  
  m2.setSpeed(speednum);
  
  
  
  }

void No_2(){

  DC_Control2();
  
  }

void No_3(){
  
  DC_Control1();
  
  }

void No_5(){

  DC_Control1();
  
  
  }

void No_6(){
  
  DC_Control1();
  DC_Control2();
  
  }

void No_8(){
  
  DC_Control2();
  
  }

void No_9(){

  DC_Control1();
  DC_Control2();
  
  }

void No_11(){

  DC_Control1();
  DC_Control2();
  
  
  }



void Callback(const std_msgs::Int16& a){
     
      triger = a.data;
     
     
}

ros::Subscriber<std_msgs::Int16> dc("dc",Callback);


void setup() {

  Wire.begin();
  delay(1000);

  nh.initNode();

  nh.subscribe(dc);
  // put your setup code here, to run once:

}

void loop() {

  if(triger == 2){

    No_2();
    
    }

   if(triger == 3){

    No_3();
    
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

   if(triger == 9){

    No_9();
    
    }



   if(triger == 11){

    No_11();
    
    }


  nh.spinOnce();

  // put your main code here, to run repeatedly:

}
