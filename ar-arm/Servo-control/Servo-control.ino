#include <Servo.h>
#define L_arm1_PIN 13
#define L_arm2_PIN 12
#define S_arm1_PIN 11
#define S_arm2_PIN 10

#define L1_INIT 150
#define L2_INIT 180
#define S1_INIT 0
#define S2_INIT 0


Servo long_arm1;
Servo long_arm2;
Servo short_arm1;
Servo short_arm2;




void setup() {
  pinMode(L_arm1_PIN,OUTPUT);
  pinMode(L_arm2_PIN,OUTPUT);
  pinMode(S_arm1_PIN,OUTPUT);
  pinMode(S_arm2_PIN,OUTPUT);
  long_arm1.attach(L_arm1_PIN);
  long_arm2.attach(L_arm2_PIN);
  short_arm1.attach(S_arm1_PIN);
  short_arm2.attach(S_arm2_PIN);
  
  //long_arm1.write(L1_INIT);
  //long_arm2.write(L2_INIT);
  //short_arm1.write(S1_INIT);
  //short_arm2.write(S2_INIT);
  
  
  // put your setup code here, to run once:

}

void loop() {

  long_arm1.write(L1_INIT);
  //long_arm2.write(L2_INIT);
  //short_arm1.write(S1_INIT);
  //short_arm2.write(S2_INIT);
  // put your main code here, to run repeatedly:

}
