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

#define I2C_ADDR 0x10
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

void InitAdc(void){
	
	//Clear or AREF Internal Vref turned off,
	ADMUX &= ~(0b11000000);
	//AVCC with external capacitor at AREF pin,
	ADMUX |= 0b01000000;
	//Internal 1.1V Voltage Reference with external capacitor at AREF pin
	//ADMUX |= 0b11000000;
	
	//set ADLAR 1 to left adjust the result
	//ADMUX |= 0b00100000;
	
	//ADCSRA Settings
	//ADEN(ADC Enable) = 1,
	//ADSC(ADC Start Conversion) = 1
	//ADATE(ADC Auto Trigger Enable) = 0
	//ADIF(ADC Interrupt Flag) = 0
	//ADIE(ADC Interrupt Enable) = 0
	//ADPS[2:0](ADC Prescaler Select Bits) = {0,0,0}
	ADCSRA = (1<<ADEN);
	
	//DIDR0 Settings, Digital Input Disable Register 0
	//DIDR0 = 0b00111111;
	
}

unsigned int ConvAdc(unsigned char chAD){
	unsigned int dataADC = 50;
	ADMUX = (ADMUX & 0xF0) | chAD;
	ADCSRA |= (1<<ADSC); //ADSC = 1: ADC Start Conversion
	while(ADCSRA & (1<<ADSC));
	//If ADLAR = 0,
	dataADC = ADCL;
	dataADC += (ADCH<<8);
	//If ADLAR = 1,
	//dataADC = (ADCL>>6);
	//dataADC += (ADCH<<2);
	return dataADC;
}

void setup (){
	
	DDRB = 0b11111111;
	DDRC = 0b00000000;
	DDRD = 0b11111111;

	// LED of addresses 0x10~ 0x1F 0~F -> 0~15
	//PORTD |= (0b00001111 & I2C_ADDR);

	// xxx is it needed to set i2c pin?
	TI2C_init(I2C_ADDR,i2c_received_cb, i2c_request_cb);
	
	InitAdc();
}

int main(void)
{
	setup();
    while(1)
    {
        //xxx Read Line sensors (ADC) 
		 sensor_values[0] = ConvAdc(0);
	     sensor_values[1] = ConvAdc(1);
		 sensor_values[2] = ConvAdc(2);
		 sensor_values[3] = ConvAdc(3);
		_delay_ms(10);
    }
}