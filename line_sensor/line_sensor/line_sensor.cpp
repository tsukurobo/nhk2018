/*
 * I2C_test.cpp
 *
 * Created: 2018/01/21 18:56:19
 * Author : ip.sakura
 */ 

//motordriver_I2C

//I2C
#define F_CPU 1000000UL

#include <avr/io.h>
#include "I2CSlave.h"
#include "TI2C.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>


#define I2C_ADDR 0x10

volatile uint8_t data; //®”0`255‚ğudatav‚ÆŒÄ‚Ñ‚Ü‚·


//// lcd
void initHard();
char str[] = "hello avr world";


void i2c_received_cb(char* str) {
}

void i2c_request_cb(char* buf) {
	// set TI2C_buf_send.str_buf
	strcpy(buf, "Hello World\n");  
}

void setup()
{
	TI2C_init(I2C_ADDR,i2c_received_cb, i2c_request_cb);

}

int main()
{
	setup();

	// Main program loop
	while(1){
	}
}
