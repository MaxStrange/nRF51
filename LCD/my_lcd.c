#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf_delay.h"
#include "my_lcd.h"
#include "my_led.h"

/*Function declarations - private helpers*/
static void pulse(void);
static void send(uint8_t data);
static void send_4_bits(uint8_t data);

/*
Initializes the actual lcd module.
*/
void lcd_begin(void)
{
  LCD_ALERT_SEND_COMMAND();

  //delay at least 40ms after power rises above 2.7v
  nrf_delay_ms(50);

  /*
  Now we want to initialize the lcd according to the
  datasheet specifications (p. 46, 4-bit interface
  initialization)
  */

  send_4_bits(0x03);
  nrf_delay_ms(5);

  send_4_bits(0x03);
  nrf_delay_ms(5);

  send_4_bits(0x03);
  nrf_delay_us(150);

  send_4_bits(0x02);

  /*
  From here on, we can now use the send() Function
  to send 8 bit commands over the four bit
  interface.
  */

  /*
  Finish initializing the lcd. See p.46 of datasheet.
  */

  nrf_delay_ms(1);
  send(LCD_CMND_TWO_LINES);//2 lines of display

  nrf_delay_ms(1);
  send(LCD_CMND_DSPLY_OFF);//display off

  nrf_delay_ms(1);
  send(LCD_CMND_CLR_DSPLY);//clear display

  nrf_delay_ms(1);
  send(LCD_CMND_SHIFT_MODE_LEFT);//have display shift left when adding chars - see p.26

  nrf_delay_ms(1);
  send(LCD_CMND_DSPLY_ON_NO_BLINK);//Display on, no blink
}

/*
Moves the cursor to the location specified,
using zero-based indexing.
*/
void lcd_goto(uint8_t x, uint8_t y)
{
  LCD_ALERT_SEND_COMMAND();
  if (x == 0)
    send(0x80 + y);
  else
    send(0xC0 + y);
}

/*
Initializes the pin configuration for the lcd module.
*/
void lcd_init(void)
{
  NRF_GPIO->DIRSET = LCDPIN_E | LCDPIN_RS |
                     LCDPIN_D4 | LCDPIN_D5 | LCDPIN_D6 | LCDPIN_D7;
}

void lcd_write_char(uint8_t c)
{
  LCD_ALERT_SEND_CHARACTER();
  send(c);
}

void lcd_write_str(const char *str)
{
  LCD_ALERT_SEND_CHARACTER();
  while (*str != '\0')
  {
    send(*str++);
  }
}


/*
Private methods
*/



static void pulse(void)
{
  NRF_GPIO->OUTSET = LCDPIN_E;
  nrf_delay_us(20);
  NRF_GPIO->OUTCLR = LCDPIN_E;
  nrf_delay_us(20);
}

static void send(uint8_t data)
{
  //send the upper four bits
  uint8_t upper_bits = (0xF0 & data) >> 4;
  send_4_bits(upper_bits);
  //send the lower four bits
  send_4_bits(data);
}

/*
A function to send only the lower four bits of
a byte.
*/
static void send_4_bits(uint8_t data)
{
  NRF_GPIO->OUTCLR = LCDPIN_D4 | LCDPIN_D5 | LCDPIN_D6 | LCDPIN_D7;
  NRF_GPIO->OUTSET = (data & 0x0F) << _LCD_PIN_NUM_D4;
  NRF_GPIO->OUTCLR = (~data & 0x0F) << _LCD_PIN_NUM_D4;
  pulse();
}
