#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf_delay.h"
#include "my_led.h"

int main(void)
{
  led_init();
  lcd_init();
  lcd_begin();

  while (true)
  {
    static char *lcd_str = "hello";
    lcd_goto(0, 0);
    lcd_write_str(lcd_str);

    nrf_delay_ms(1000);
  }
}
