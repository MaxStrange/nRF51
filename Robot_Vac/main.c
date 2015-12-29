#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"

#include "my_led.h"
#include "my_uart.h"
#include "console.h"
#include "my_lcd.h"



int main(void)
{
    led_init();
    uart_init();
    lcd_init();
    lcd_begin();

    while (true)
    {
      bool worked = console_go();
      if (!worked)
      {
        const char *help_str = console_get_help_str();
        uart_write_str(help_str);
      }

      // const char *help_str = console_get_help_str();
      // uart_write_str(help_str);
      // nrf_delay_ms(1000);
      // led_pattern_dance();

      // const char *help_str = console_get_help_str();
      // lcd_write_str(help_str);
      // nrf_delay_ms(1000);
      // led_pattern_dance();
    }
}
