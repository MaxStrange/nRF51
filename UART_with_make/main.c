#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf_delay.h"
#include "my_led.h"
#include "my_uart.h"


int main(void)
{
    led_init();
    uart_init();

    while (true)
    {
      led_pattern_dance();
      nrf_delay_ms(500);
      uart_write_str("hello");
    }
}
