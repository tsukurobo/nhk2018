/*
 * lcd.h
 *
 * Created: 2018/03/23 22:40:58
 *  Author: magon
 */ 


#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

// Command Instruction code定義
#define LCD_CLEAR           0b00000001
#define LCD_RTN_HOME        0b00000010

#define LCD_ENTRY           0b00000100
#define LCD_ENTRY_ID_DEC    0b0000
#define LCD_ENTRY_ID_INC    0b0010
#define LCD_ENTRY_S_NO      0b0000
#define LCD_ENTRY_S_YES     0b0001

#define LCD_DISPLAY         0b00001000
#define LCD_DISP_OFF        0b0000
#define LCD_DISP_ON         0b0100
#define LCD_DISP_CUR_OFF    0b0000
#define LCD_DISP_CUR_ON     0b0010
#define LCD_DISP_BLK_OFF    0b0000
#define LCD_DISP_BLK_ON     0b0001

#define LCD_FUNCTION_DL_8   0b00110000
#define LCD_FUNCTION_DL_4   0b00100000
#define LCD_FUNCTION        LCD_FUNCTION_DL_4   // データ長4bitで使用
#define LCD_FUNCTION_N_1    0b0000
#define LCD_FUNCTION_N_2    0b1000
#define LCD_FUNCTION_F_5x8  0b0000
#define LCD_FUNCTION_F_5x11 0b0100

#define LCD_DD_ADDR         0b10000000

// 接続情報
#define LCD_PORT_CTRL   PORTD   // E,RS,R/W信号に使用するポートを指定
#define LCD_PORT_DATA   PORTD   // データーポートを指定
#define LCD_DDR_CTRL    DDRD    // E,RS,R/W信号に使用するポートのDDR
#define LCD_DDR_DATA    DDRD    // データーポートのDDR
#define LCD_BIT_E       0b01000000  // Enableビット指定
#define LCD_BIT_RS      0b00100000  // Register Selectビット指定
#define LCD_BIT_RW      0b00010000  // R/Wビット指定
#define LCD_BIT_DATA    0b00001111  // データビット指定

// Register Select
#define LCD_RS_CTRL ~LCD_BIT_RS
#define LCD_RS_DATA LCD_BIT_RS

// LCDに対するデータ出力
void lcd_write_(unsigned char, unsigned char);

// LCDに対するデータ出力(4bit長用)
void lcd_write(unsigned char, unsigned char);

// LCDデータ長4bit指定
void lcd_setDL4Bit();

// LCD Entry Mode
void lcd_entryMode(unsigned char);

// LCD Display Set
void lcd_display(unsigned char);

// LCD Clear Display
void lcd_clear();

// LCD Function Set
void lcd_setFunction(unsigned char);

// カーソル移動
void lcd_moveCursor(unsigned char, unsigned char);

// LCD初期化
void lcd_init();

// LCD 文字列表示
void lcd_print(char*);

// 数値を表示する(2Byte)
void lcd_printInt(unsigned int);

// 数値を表示する(1Byte)
void lcd_printByte(unsigned char);

#endif // LCD_H_INCLUDED