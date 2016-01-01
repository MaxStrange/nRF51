#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"

#include "my_lcd.h"
#include "my_led.h"
#include "my_strings.h"

/*Function declarations - private helpers*/
static void pulse(void);
static void send(uint8_t data);
static void send_4_bits(uint8_t data);
static void write_all_chars_in_str(volatile const char *str);

/*Internal state*/
static volatile char i_as_str[6];//buffer for write_int function//TODO: really don't want this to be accessed by interrupts


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

void lcd_clear(void)
{
  LCD_ALERT_SEND_COMMAND();

  nrf_delay_ms(1);
  send(LCD_CMND_DSPLY_OFF);//display off

  nrf_delay_ms(1);
  send(LCD_CMND_CLR_DSPLY);//clear display

  nrf_delay_ms(1);
  send(LCD_CMND_SHIFT_MODE_LEFT);//have display shift left when adding chars - see p.26

  nrf_delay_ms(1);
  send(LCD_CMND_DSPLY_ON_NO_BLINK);//Display on, no blink
}

void lcd_clear_and_write(const char *str)
{
  lcd_clear();
  lcd_goto(0, 0);
  lcd_write_str(str);
}

/*
Moves the cursor to the location specified,
using zero-based indexing.
*/
void lcd_goto(uint8_t x, uint8_t y)
{
  LCD_ALERT_SEND_COMMAND();
  if (y == 0)
    send(0x80 + x);
  else
    send(0xC0 + x);
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

void lcd_write_int(uint16_t i)
{
  strings_int_to_str(i, i_as_str);
  volatile char *str = i_as_str;
  write_all_chars_in_str(str);
}

void lcd_write_str(const char *str)
{
  /*
  Determine how long the string is
  */
  uint32_t len = strings_get_length(str);

  /*
  If the string is short enough, just display it on the
  first line.
  */
  if (len <= 15)
  {
    write_all_chars_in_str(str);
  }
  else if (len <= 31)
  {//if the string is short enough to display on both lines, do it
    static char buffer_right[16];
    static char buffer_left[16];

    //reinitialize the buffers
    for (uint8_t i = 0; i < 16; i++)
    {
      buffer_left[i] = 0;
      buffer_right[i] = 0;
    }

    strings_split(15, str, buffer_left, buffer_right);
    write_all_chars_in_str(buffer_left);
    lcd_goto(0, 1);
    write_all_chars_in_str(buffer_right);
  }
  else
  {//string is too long, have to scroll the lcd //TODO
    lcd_write_str("string too long.");
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

static void write_all_chars_in_str(volatile const char *str)
{
  LCD_ALERT_SEND_CHARACTER();
  while (*str != '\0')
  {
    send(*str++);
  }
}
