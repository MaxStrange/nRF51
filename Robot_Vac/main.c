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


int main(void)
{
    clock_init();
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
        uart_write_str(" ");
      }
    }
}
