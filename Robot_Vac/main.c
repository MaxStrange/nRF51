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
    timer_count_to(100);

    while (true)
    {
    //   bool worked = console_go();
    //   if (!worked)
    //   {
    //     const char *help_str = console_get_help_str();
    //     uart_write_str(help_str);
    //   }
    // uint32_t seconds_left = timer_get_seconds_left();
    // lcd_clear();
    // lcd_goto(0, 0);
    // lcd_write_int(seconds_left);
    }
}
