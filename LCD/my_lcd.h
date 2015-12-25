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

/*Macro functions used by the LCD module*/
#define LCD_ALERT_SEND_COMMAND() (NRF_GPIO->OUTCLR |= LCDPIN_RS)//low sig on rs
#define LCD_ALERT_SEND_CHARACTER() (NRF_GPIO->OUTSET |= LCDPIN_RS)//high sig

/*Function declarations - API methods*/
void lcd_begin(void);
void lcd_goto(uint8_t x, uint8_t y);
void lcd_init(void);
void lcd_write_char(uint8_t c);
void lcd_write_str(const char *str);

/*Function declarations - private helpers*/
static void pulse(void);
static void send(uint8_t data);
static void send_4_bits(uint8_t data);

#endif //MY_LCD_H
