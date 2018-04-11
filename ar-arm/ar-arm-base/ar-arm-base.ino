#include <ros.h>

#include <std_msgs/Int8.h>

#include <std_msgs/Int16.h>

volatile int triger = 0;

ros::NodeHandle nh;

void No_2(){}

void No_3(){}

void No_5(){}

void No_6(){}

void No_8(){}

void No_9(){}

void No_11(){}



void Callback(const std_msgs::Int16& a){
     
      triger = a.data;
     
     
}

ros::Subscriber<std_msgs::Int16> num("num",Callback);


void setup() {

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
