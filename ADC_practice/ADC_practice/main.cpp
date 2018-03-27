/*
 * ADC_practice.cpp
 *
 * Created: 2018/03/23 22:34:01
 * Author : yoshihara
 *参考http://ziqoo.com/wiki/index.php?AVR%20A%2FD%CA%D1%B4%B9
 */ 

#ifndef F_CPU
#define F_CPU 1000000UL    // 動作周波数に1MHzを指定
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"

int result = ADC;

void i2c_received_cb(char* str) {
}

void i2c_request_cb(char* buf) {
	sprintf(buf, "%d",result);
}


void adc_init(){
	ADMUX  = 0b00010000;  //pc0を使用するため
	ADCSRA = 0b11010111;
	DIDR0  = 0b00000001;
}

void adc(){
	ADCSRA |= _BV(ADIF);
	ADCSRA |= _BV(ADSC);
}

int main(void){
	setup();
	adc_init();
	
	
	while(1){
		adc();
		
		
		// ADC完了待ち
		loop_until_bit_is_set(ADCSRA, ADIF);
		
		// ADC結果取得
		
		
		
		
		_delay_ms(100);
	}
}



