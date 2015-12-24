#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "my_led.h"
#include "boards.h"
#include "nrf_delay.h"

void led_blink_all(void)
{
  NRF_GPIO->OUT ^= LED1 | LED2 | LED3 | LED4;
}

void led_init(void)
{
  NRF_GPIO->DIR |= LED1 | LED2 | LED3 | LED4;
}

void led_pattern_dance(void)
{
  NRF_GPIO->OUT ^= LED1 | LED4;
  nrf_delay_ms(100);
  NRF_GPIO->OUT ^= LED2 | LED3;
  nrf_delay_ms(100);
  NRF_GPIO->OUT ^= LED1 | LED2;
  nrf_delay_ms(100);
  NRF_GPIO->OUT ^= LED1 | LED3;
  nrf_delay_ms(50);
  NRF_GPIO->OUT ^= LED2 | LED3;
  nrf_delay_ms(50);
  NRF_GPIO->OUT ^= LED4 | LED1;
  nrf_delay_ms(50);
  NRF_GPIO->OUT ^= LED3 | LED2 | LED4;
  nrf_delay_ms(50);
  NRF_GPIO->OUT ^= LED1 | LED2;
  nrf_delay_ms(50);
}
