/*
 * adc.h
 *
 * Created: 2018/03/23 22:43:27
 *  Author: magon
 */ 


#ifndef ADC_H_
#define ADC_H_

// Voltage Reference Selections
#define ADC_REFS_AREFF  0x00                        // 00
#define ADC_REFS_AVCC   (_BV(REFS0))                // 01
#define ADC_REFS_IN     (_BV(REFS1) | _BV(REFS0))   // 11

// Input Channel Selections
#define ADC_MUX_ADC0    0x00                    // 0000
#define ADC_MUX_ADC1    (_BV(MUX0))             // 0001
#define ADC_MUX_ADC2    (_BV(MUX1))             // 0010
#define ADC_MUX_ADC3    (_BV(MUX1) | _BV(MUX0)) // 0011
#define ADC_MUX_ADC4    (_BV(MUX2))             // 0100
#define ADC_MUX_ADC5    (_BV(MUX2) | _BV(MUX0)) // 0101
#define ADC_MUX_VBG     (_BV(MUX3) | _BV(MUX2) | _BV(MUX1))             // 1110
#define ADC_MUX_GND     (_BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0)) // 1111

// ADC Prescaler Selections(Division Factor)
#define ADC_ADPS_2      (_BV(ADPS0))                            // 001
#define ADC_ADPS_4      (_BV(ADPS1))                            // 010
#define ADC_ADPS_8      (_BV(ADPS1) | _BV(ADPS0))               // 011
#define ADC_ADPS_16     (_BV(ADPS2))                            // 100
#define ADC_ADPS_32     (_BV(ADPS2) | _BV(ADPS0))               // 101
#define ADC_ADPS_64     (_BV(ADPS2) | _BV(ADPS1))               // 110
#define ADC_ADPS_128    (_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS1))  // 111




#endif /* ADC_H_ */