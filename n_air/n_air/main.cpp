/*
 * n_air.cpp
 *
 * Created: 2018/03/17 0:00:44
 * Author : yoshihara
 */ 

#define F_CPU 20000000UL

#include <avr/io.h>
#include "I2CSlave.h"
#include "TI2C.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define I2C_ADDR 0x14

bool states[8];

void on1(){
	PORTD |= 0b00010000;//valve1A high
	PORTD &= 0b11011111;//valve1B low
	states[0] = true;
}

void off1(){
	PORTD &= 0b11101111;//valve1A low
	PORTD |= 0b00100000;//valve1B high
	states[0] = false;
}

void on2(){
	PORTD |= 0b01000000;//valve2A high
	PORTD &= 0b01111111;//valve2B low
	states[1] = true;
}

void off2(){
	PORTD &= 0b10111111;//valve2A low
	PORTD |= 0b10000000;//valve2B high
	states[1] = false;
}

void on3(){
	PORTB |= 0b00000001;//valve3A high
	PORTB &= 0b11111101;//valve3B low
	states[2] = true;
}

void off3(){
	PORTB &= 0b11111110;//valve3A low
	PORTB |= 0b00000010;//valve3B high
	states[2] = false;
}

void on4(){
	PORTB |= 0b00000100;//valve4A high
	PORTB &= 0b11110111;//valve4B low
	states[3] = true;
}

void off4(){
	PORTB &= 0b11111011;//valve4A low
	PORTB |= 0b00001000;//valve4B high
	states[3] = false;
}

void on5(){
	PORTB |= 0b00010000;//valve5A high
	PORTB &= 0b11011111;//valve5B low
	states[4] = true;
}

void off5(){
	PORTB &= 0b11101111;//valve5A low
	PORTB |= 0b00100000;//valve5B high
	states[4] = false;
}

void on6(){
	PORTB |= 0b01000000;//valve6A high
	PORTB &= 0b01111111;//valve6B low
	states[5] = true;
}

void off6(){
	PORTB &= 0b10111111;//valve6A low
	PORTB |= 0b10000000;//valve6B high
	states[5] = false;
}

void on7(){
	PORTC |= 0b00000001;//valve7A high
	PORTC &= 0b11111101;//valve7B low
	states[6] = true;
}

void off7(){
	PORTC &= 0b11111110;//valve7A low
	PORTC |= 0b00000010;//valve7B high
	states[6] = false;
}

void on8(){
	PORTC |= 0b00000100;//valve8A high
	PORTC &= 0b11110111;//valve8B low
	states[7] = true;
}

void off8(){
	PORTC &= 0b11111011;//valve8A low
	PORTC |= 0b00001000;//valve8B high
	states[7] = false;
}

void init(){
	on1();
	on2();
	on3();
	on4();
	on5();
	on6();
	on7();
	on8();
}


void toggle1(){
	if (states[0]){
		off1();
	}
	else if(!states[0]){
		on1();
	}
}
void toggle2(){
	if (states[1]){
		off2();
	}
	else if(!states[1]){
		on2();
	}
	
}
void toggle3(){
	if (states[2]){
		off3();
	}
	else if(!states[2]){
		on3();
	}
}
void toggle4(){
	if (states[3]){
		off4();
	}
	else if(!states[3]){
		on4();
	}
}
void toggle5(){
	if (states[4]){
		off5();
	}
	else if(!states[4]){
		on5();
	}
}
void toggle6(){
	if (states[5]){
		off6();
	}
	else if(!states[5]){
		on6();
	}
}
void toggle7(){
	if (states[6]){
		off7();
	}
	else if(!states[6]){
		on7();
	}
}
void toggle8(){
	if (states[7]){
		off8();
	}
	else if(!states[7]){
		on8();
	}
}



void i2c_received_cb(char* str) {

	int n = atoi(str);
	
	if (n == 0){
		init();
	}
	
	else if (n == 1){
		toggle1();
	}
	else if(n == 2){
		toggle2();
	}
	else if(n == 3){
		toggle3();
	}
	else if(n == 4){
		toggle4();
	}
	else if(n == 5){
		toggle5();
	}
	else if(n == 6){
		toggle6();
	}
	else if(n == 7){
		toggle7();
	}
	else if(n == 8){
		toggle8();
	}
}


void i2c_request_cb(char* buf) {
	// set TI2C_buf_send.str_buf
	//strcpy(buf, "Hello World\n");
	//sprintf(buf,"%ld",count);
}

void setup (){
	
	DDRB = 0b11111111;
	DDRC = 0b00001111;
	DDRD = 0b11111111;

	// LED of addresses 0x10~ 0x1F 0~F -> 0~15
	PORTD |= (0b00001111 & I2C_ADDR);
	
    init();
	
	TI2C_init(I2C_ADDR,i2c_received_cb, i2c_request_cb);
}




int main(void){
	setup();
	
	/* Replace with your application code */
	while (1){ 
	}
}

