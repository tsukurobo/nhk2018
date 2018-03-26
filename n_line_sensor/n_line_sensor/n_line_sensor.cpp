/*
 * n_line_sensor.cpp
 *
 * Created: 2018/03/24 15:21:10
 *  Author: ip.sakura
 */ 


#define F_CPU 1000000UL

#include <avr/io.h>
#include "I2CSlave.h"
#include "TI2C.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SENSOR_SUM 4

#define I2C_ADDR 0x21
int sensor_values[SENSOR_SUM] = {};

void i2c_received_cb(char* str) {
	//pass 
}

void i2c_request_cb(char* buf) {
	// set TI2C_buf_send.str_buf
	//strcpy(buf, "Hello World\n");
	//sprintf(buf,"%ld",count);
	sprintf(buf, "%d,%d,%d,%d", sensor_values[0],sensor_values[1],sensor_values[2],sensor_values[3]);
}

void setup (){
	
	DDRB = 0b11111111;
	DDRC = 0b00001111;
	DDRD = 0b11111111;

	// LED of addresses 0x10~ 0x1F 0~F -> 0~15
	//PORTD |= (0b00001111 & I2C_ADDR);

	// xxx is it needed to set i2c pin?
	TI2C_init(I2C_ADDR,i2c_received_cb, i2c_request_cb);
}

int main(void)
{
	setup();
    while(1)
    {
        //xxx Read Line sensors (ADC) 
		 sensor_values[0] = 9;
	     sensor_values[1] = 8;
		 sensor_values[2] = 7;
		 sensor_values[3] = 6;
		_delay_ms(10);
    }
}