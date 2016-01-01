#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"

#include "my_led.h"
#include "my_uart.h"
#include "console.h"
#include "my_lcd.h"
#include "my_timer.h"
#include "my_clock.h"
#include "my_strings.h"
#include "range_finder.h"

// static char buffer[33];//lcd is 16x2 plus one for the nul
// static char user_in[6];//array to take user's input for the timer


int main(void)
{
    clock_init();
    led_init();
    uart_init();
    lcd_init();
    lcd_begin();
    range_finder_init();

    while (true)
    {
      // bool worked = console_go();
      // if (!worked)
      // {
      //   const char *help_str = console_get_help_str();
      //   uart_write_str(help_str);
      //   uart_write_str(" ");
      // }

      uint8_t dist = range_finder_get_distance();
      uart_write_str("Distance to nearest object: ");
      uart_write_int(dist);
      uart_write_str(" ");

      nrf_delay_ms(1000);
    }
}
