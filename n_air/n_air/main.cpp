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


void i2c_received_cb(char* str) {

	int n = atoi(str);
	
	if(n==0){
		PORTD |= 0b00010000;//valve1A high
	}
	else if(n==1){
		PORTD |= 0b00100000;//valve1B high
	}
	else if(n==2){
		PORTD |= 0b01000000;//valve2A high
	}
	else if(n==3){
		PORTD |= 0b10000000;//valve2B high
	}
	else if(n==4){
		PORTB |= 0b00000001;//valve3A high
	}
	else if(n==5){
		PORTB |= 0b00000010;//valve3B high
	}
	else if(n==6){
		PORTB |= 0b00000100;//valve4A high
	}
	else if(n==7){
		PORTB |= 0b00001000;//valve4B high
	}
	else if(n==8){
		PORTB |= 0b00010000;//valve5A high
	}
	else if(n==9){
		PORTB |= 0b00100000;//valve5B high
	}
	else if(n==10){
		PORTB |= 0b01000000;//valve6A high
	}
	else if(n==11){
		PORTB |= 0b10000000;//valve6B high
	}
	else if(n==12){
		PORTC |= 0b00000001;//valve7A high
	}
	else if(n==13){
		PORTC |= 0b00000010;//valve7B high
	}
	else if(n==14){
		PORTC |= 0b00000100;//valve8A high
	}
	else if(n==15){
		PORTC |= 0b00001000;//valve8B high
	}
	else if(n==16){
		PORTD &= 0b11101111;//valve1A low
	}
	else if(n==17){
		PORTD &= 0b11011111;//valve1B low
	}
	else if(n==18){
		PORTD &= 0b10111111;//valve2A low
	}
	else if(n==19){
		PORTD &= 0b01111111;//valve2B low
	}
	else if(n==20){
		PORTB &= 0b11111110;//valve3A low
	}
	else if(n==21){
		PORTB &= 0b11111101;//valve3B low
	}
	else if(n==22){
		PORTB &= 0b11111011;//valve4A low
	}
	else if(n==23){
		PORTB &= 0b11110111;//valve4B low
	}
	else if(n==24){
		PORTB &= 0b11101111;//valve5A low
	}
	else if(n==25){
		PORTB &= 0b11011111;//valve5B low
	}
	else if(n==26){
		PORTB &= 0b10111111;//valve6A low
	}
	else if(n==27){
		PORTB &= 0b01111111;//valve6B low
	}
	else if(n==28){
		PORTC &= 0b11111110;//valve7A low
	}
	else if(n==29){
		PORTC &= 0b11111101;//valve7B low
	}
	else if(n==30){
		PORTC &= 0b11111011;//valve8A low
	}
	else if(n==31){
		PORTC &= 0b11110111;//valve8B low
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

	
	TI2C_init(I2C_ADDR,i2c_received_cb, i2c_request_cb);
}


int main(void)
{
	setup();
	
	/* Replace with your application code */
	while (1)
	{

	}
}

