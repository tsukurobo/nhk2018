/*
 * lcd.c
 *
 * Created: 2018/03/23 22:44:54
 *  Author: yoshihara
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL    // ������g����16MHz���w��
#endif
 
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
 
// LCD �s�ɑΉ�����DDRAM�J�n�A�h���X
const unsigned char lcd_ddram_addr_row[] = {0x0, 0x40, 0x14, 0x54};
 
/************************************************************************/
/* LCD�ɑ΂���f�[�^�o��                                                */
/************************************************************************/
void lcd_write_(unsigned char rs, unsigned char data) {
    // R/W -> W
    LCD_PORT_CTRL &= ~LCD_BIT_RW;
     
    // RS���Z�b�g
    if (rs == LCD_RS_DATA) {
        LCD_PORT_CTRL |= rs;
    } else {
        LCD_PORT_CTRL &= rs;
    }
     
    // E -> H
    LCD_PORT_CTRL |= LCD_BIT_E;
     
    // �f�[�^�o��
    LCD_PORT_DATA = (LCD_PORT_DATA & ~LCD_BIT_DATA) | (data & LCD_BIT_DATA);
 
    // E -> L
    LCD_PORT_CTRL &= ~LCD_BIT_E;
}
 
/************************************************************************/
/* LCD�ɑ΂���f�[�^�o��(4bit��)                                        */
/************************************************************************/
void lcd_write(unsigned char rs, unsigned char data) {
    lcd_write_(rs, data >> 4);
    _delay_us(50);
    lcd_write_(rs, data);
}
 
/************************************************************************/
/* LCD�f�[�^��4bit�w��                                                  */
/************************************************************************/
void lcd_setDL4Bit() {
    // 4bit mode
 
    lcd_write_(LCD_RS_CTRL, LCD_FUNCTION_DL_8 >> 4);
    _delay_ms(5);
    lcd_write_(LCD_RS_CTRL, LCD_FUNCTION_DL_8 >> 4);
    _delay_us(150);
     
    lcd_write_(LCD_RS_CTRL, LCD_FUNCTION_DL_8 >> 4);
    _delay_us(50);
 
    lcd_write_(LCD_RS_CTRL, LCD_FUNCTION_DL_4 >> 4);
    _delay_us(50);
}
 
/************************************************************************/
/* LCD Entry Mode                                                       */
/************************************************************************/
void lcd_entryMode(unsigned char data) {
    lcd_write(LCD_RS_CTRL, LCD_ENTRY | data);
    _delay_us(50);
}
 
/************************************************************************/
/* LCD Display Set                                                      */
/************************************************************************/
void lcd_display(unsigned char data) {
    lcd_write(LCD_RS_CTRL, LCD_DISPLAY | data);
    _delay_us(50);
}
 
/************************************************************************/
/* LCD Clear Display                                                    */
/************************************************************************/
void lcd_clear() {
    lcd_write(LCD_RS_CTRL, LCD_CLEAR);
    _delay_ms(5);
}
 
/************************************************************************/
/* LCD Function Set                                                     */
/************************************************************************/
void lcd_setFunction(unsigned char data) {
 
    lcd_write(LCD_RS_CTRL, LCD_FUNCTION | data);
    _delay_us(50);
}
 
/************************************************************************/
/* �J�[�\���ړ�                                                         */
/************************************************************************/
void lcd_moveCursor(unsigned char col, unsigned char row) {
    unsigned char addr = lcd_ddram_addr_row[row] + col;
    lcd_write(LCD_RS_CTRL, LCD_DD_ADDR | addr);
    _delay_us(50);
}
 
/************************************************************************
 * LCD������
 * 4bit Mode
 ************************************************************************/
void lcd_init() {
    // LCD�Ŏg�p����|�[�g�̃r�b�g���o�͂Ɏw��
    LCD_DDR_CTRL = LCD_DDR_CTRL | LCD_BIT_E | LCD_BIT_RS | LCD_BIT_RW;
    LCD_DDR_DATA = LCD_DDR_CTRL | LCD_BIT_DATA;
    _delay_ms(50);
 
    // �f�[�^��4bit�w��
    lcd_setDL4Bit();
    // Function�Z�b�g display line = 2, font = 5x8dots
    lcd_setFunction(LCD_FUNCTION_N_2 | LCD_FUNCTION_F_5x8);
    // Display�Z�b�g 
    lcd_display(LCD_DISP_ON | LCD_DISP_CUR_OFF | LCD_DISP_BLK_OFF);
    // Clear Display
    lcd_clear();
    // Entry Mode �J�[�\���ړ����� = Increment, Display shift = disable
    lcd_entryMode(LCD_ENTRY_ID_INC | LCD_ENTRY_S_NO);
}
 
/************************************************************************/
/* LCD ������o��                                                       */
/************************************************************************/
void lcd_print(char* str) {
    for (int i = 0; str[i]; i++) {
        lcd_write(LCD_RS_DATA, str[i]);
        _delay_us(50);
    }
}
// �����ɑΉ�����L�����N�^�R�[�h
char lcd_numChars[] = {"0123456789ABCDEF"};
/************************************************************************/
/* ���l��\������(2Byte)                                                */
/************************************************************************/
void lcd_printInt(unsigned int num) {
        lcd_printByte(num >> 8);
        lcd_printByte(num & 0x00FF);
}
/************************************************************************/
/* ���l��\������(1Byte)                                                */
/************************************************************************/
void lcd_printByte(unsigned char byte) {
    lcd_write(LCD_RS_DATA, lcd_numChars[byte >> 4]);
    _delay_us(50);
    lcd_write(LCD_RS_DATA, lcd_numChars[byte & 0x0F]);
    _delay_us(50);
}
