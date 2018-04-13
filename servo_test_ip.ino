#include <Servo.h>
#define beginNo 2
#define b 0
#define c 1
#define d 2
#define e 3
#define f 4
#define g 5
#define h 6

#define OPEN 90
#define CLOSE 0
#define OPEN0 90
#define CLOSE0 0

Servo servos[7];
int state[7] = {}; // open is 0, close is 1

void setupServos() {
  int i=0;
  for (i = 0; i <7; i++) {
      servos[i] = Servo();
      servos[i].attach(i+beginNo);
      if(i == 0){
        servos[i].write(OPEN0);//xxx    
      } else {
        servos[i].write(OPEN);
      }
    }
}

void toggleServo(int servoNo) {
  int i = servoNo;
  if (state[i] == 0) {// open
      if (i == 0){
        servos[i].write(CLOSE0);
        } else {
        servos[i].write(CLOSE);
      }
      state[i] = 1;
    } else if (state[i] == 1) {
      if (i == 0){
        servos[i].write(OPEN0);
      }  else {
        servos[i].write(OPEN);
      }
      state[i] = 0;
    }  
}
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println('setup');
  setupServos();
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);

  int val = -1;
  if(Serial.available() > 0) {
    val = Serial.read();
    if (val == 'b') {
      Serial.println('b');
      toggleServo(b);
    } else if (val == 'c') {
      Serial.println('c');
      toggleServo(c);
    } else if (val == 'd') {
      Serial.println('d');     
      toggleServo(d);
    }else if (val == 'e') {
      Serial.println('e');      
      toggleServo(e);
    }else if (val == 'f') {
      Serial.println('f');      
      toggleServo(f);
    }else if (val == 'g') {
      Serial.println('g');      
      toggleServo(g);
    }else if (val == 'h') {
      Serial.println('h');      
      toggleServo(h);
    }
  }
}
