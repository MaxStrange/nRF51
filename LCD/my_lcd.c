#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf_delay.h"
#include "my_lcd.h"

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
  nrf_delay_ms(1);

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
  send(0x28);//2 lines of display. See p.29 table 8.

  nrf_delay_ms(1);
  send(0x08);//display off

  nrf_delay_ms(1);
  send(0x01);//clear display

  nrf_delay_ms(1);
  send(0x06);//have display shift left when adding chars - see p.26

  nrf_delay_ms(1);
  send(0x0C);//Display on, no blink
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
  NRF_GPIO->DIRSET |= LCDPIN_E | LCDPIN_RS | LCDPIN_D4 | LCDPIN_D5 | LCDPIN_D6 | LCDPIN_D7;
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
  NRF_GPIO->OUTSET |= LCDPIN_E;
  nrf_delay_us(1);
  NRF_GPIO->OUTCLR |= LCDPIN_E;
  nrf_delay_us(1);
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
  //take only the lower four bits of data
  data &= 0x0F;
  //shift data over by LCDPIN_D4's location number of bits to align it with
  //the four gpio pins you are sending on. Then AND it with those four bits
  //to get the bit string you need to send.
  uint8_t bits_to_set = ((data << _LCD_PIN_NUM_D4) & (LCDPIN_D4 | LCDPIN_D5 | LCDPIN_D6 | LCDPIN_D7));
  NRF_GPIO->OUTSET |= bits_to_set;
  NRF_GPIO->OUTCLR |= ~bits_to_set;
  pulse();
}
