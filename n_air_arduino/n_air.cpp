#include "Arduino.h"
#include "n_air.h"
#include "ti2c.h"

NAir::NAir(uint8_t i2caddr) :
ti2c(i2caddr){
  // @param uint8_t i2caddr: i2c addr of motor driver
  //initializer
  this->addr = i2caddr;
}

void NAir::toggle(int port){
  //@param pw: -100~100
  // char buf[10];
  //sprintf(buf, "%d", power);
  this->ti2c.sendStr(buf); 
}

void NAir::init(){
    this -> ti2c.receiveStr(buf);  
}
