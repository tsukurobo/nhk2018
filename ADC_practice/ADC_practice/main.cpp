/*
 * ADC_practice.cpp
 *
 * Created: 2018/03/23 22:34:01
 * Author : yoshihara
 *参考http://ziqoo.com/wiki/index.php?AVR%20A%2FD%CA%D1%B4%B9
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL    // 動作周波数に16MHzを指定
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "adc.h"

int main(void){
	// LCD init
	 lcd_init();
	
	// ADC init
	ADMUX  = 0b00011010;
	ADCSRA = 0b11010111;
	DIDR0  = 0b00100000;
	
	while(1){
		// ADCスタート
		ADCSRA |= _BV(ADIF);
		ADCSRA |= _BV(ADSC);
		
		// ADC完了待ち
		loop_until_bit_is_set(ADCSRA, ADIF);
		
		// ADC結果取得
		int result = ADC;
		
		//lcd_moveCursor(0,0);
		lcd_printInt(result);
		
		_delay_ms(100);
	}
}



