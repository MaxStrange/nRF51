#ifndef MY_LCD_H
#define MY_LCD_H

/*Individual pin numbers - use only in this header file*/
#define _LCD_PIN_NUM_RS 1
#define _LCD_PIN_NUM_E  2
#define _LCD_PIN_NUM_D4 3
#define _LCD_PIN_NUM_D5 4
#define _LCD_PIN_NUM_D6 5
#define _LCD_PIN_NUM_D7 6

/*Control Pins*/
#define LCDPIN_RS (1 << _LCD_PIN_NUM_RS)
#define LCDPIN_E  (1 << _LCD_PIN_NUM_E)

/*Data Bus Pins*/
#define LCDPIN_D4 (1 << _LCD_PIN_NUM_D4)
#define LCDPIN_D5 (1 << _LCD_PIN_NUM_D5)
#define LCDPIN_D6 (1 << _LCD_PIN_NUM_D6)
#define LCDPIN_D7 (1 << _LCD_PIN_NUM_D7)

/*Command Codes*/
#define LCD_CMND_CLR_DSPLY          0x01
#define LCD_CMND_RTRN_HOME          0x02
#define LCD_CMND_SHIFT_MODE_LEFT    0x06
#define LCD_CMND_DSPLY_OFF          0x08
#define LCD_CMND_DSPLY_ON_NO_BLINK  0x0C
#define LCD_CMND_TWO_LINES          0x28

/*Macro functions used by the LCD module*/
#define LCD_ALERT_SEND_COMMAND() (NRF_GPIO->OUTCLR = LCDPIN_RS)//low sig on rs
#define LCD_ALERT_SEND_CHARACTER() (NRF_GPIO->OUTSET = LCDPIN_RS)//high sig

/*Function declarations - API methods*/
void lcd_begin(void);
void lcd_clear(void);
void lcd_goto(uint8_t x, uint8_t y);
void lcd_init(void);
void lcd_write_char(uint8_t c);
void lcd_write_str(const char *str);

#endif //MY_LCD_H
