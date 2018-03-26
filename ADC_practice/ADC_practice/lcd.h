/*
 * lcd.h
 *
 * Created: 2018/03/23 22:40:58
 *  Author: magon
 */ 


#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

// Command Instruction code��`
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
#define LCD_FUNCTION        LCD_FUNCTION_DL_4   // �f�[�^��4bit�Ŏg�p
#define LCD_FUNCTION_N_1    0b0000
#define LCD_FUNCTION_N_2    0b1000
#define LCD_FUNCTION_F_5x8  0b0000
#define LCD_FUNCTION_F_5x11 0b0100

#define LCD_DD_ADDR         0b10000000

// �ڑ����
#define LCD_PORT_CTRL   PORTD   // E,RS,R/W�M���Ɏg�p����|�[�g���w��
#define LCD_PORT_DATA   PORTD   // �f�[�^�[�|�[�g���w��
#define LCD_DDR_CTRL    DDRD    // E,RS,R/W�M���Ɏg�p����|�[�g��DDR
#define LCD_DDR_DATA    DDRD    // �f�[�^�[�|�[�g��DDR
#define LCD_BIT_E       0b01000000  // Enable�r�b�g�w��
#define LCD_BIT_RS      0b00100000  // Register Select�r�b�g�w��
#define LCD_BIT_RW      0b00010000  // R/W�r�b�g�w��
#define LCD_BIT_DATA    0b00001111  // �f�[�^�r�b�g�w��

// Register Select
#define LCD_RS_CTRL ~LCD_BIT_RS
#define LCD_RS_DATA LCD_BIT_RS

// LCD�ɑ΂���f�[�^�o��
void lcd_write_(unsigned char, unsigned char);

// LCD�ɑ΂���f�[�^�o��(4bit���p)
void lcd_write(unsigned char, unsigned char);

// LCD�f�[�^��4bit�w��
void lcd_setDL4Bit();

// LCD Entry Mode
void lcd_entryMode(unsigned char);

// LCD Display Set
void lcd_display(unsigned char);

// LCD Clear Display
void lcd_clear();

// LCD Function Set
void lcd_setFunction(unsigned char);

// �J�[�\���ړ�
void lcd_moveCursor(unsigned char, unsigned char);

// LCD������
void lcd_init();

// LCD ������\��
void lcd_print(char*);

// ���l��\������(2Byte)
void lcd_printInt(unsigned int);

// ���l��\������(1Byte)
void lcd_printByte(unsigned char);

#endif // LCD_H_INCLUDED