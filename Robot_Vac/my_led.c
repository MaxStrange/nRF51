#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "boards.h"
#include "nrf_delay.h"

#include "my_led.h"

void TIMER0_IRQHandler(void);

void led_all_off(void)
{
  NRF_GPIO->OUT |= LED1 | LED2 | LED3 | LED4;
}

void led_blink_all(void)
{
  NRF_GPIO->OUT ^= LED1 | LED2 | LED3 | LED4;
}

void led_init(void)
{
  NRF_GPIO->DIR |= LED1 | LED2 | LED3 | LED4;
}

void led_pattern_breathe(void)
{
  //set up the timer
  NRF_TIMER0->CC[0] = 23456;

  //hook up the led pin to the timer's capture compare
  NRF_TIMER0->INTENSET = (1 << 16);//enable interrupt on copmare on chanel 0

  NVIC_ClearPendingIRQ(TIMER0_IRQn);
  NVIC_SetPriority(TIMER0_IRQn, 3);
  NVIC_EnableIRQ(TIMER0_IRQn);

  NRF_TIMER0->TASKS_START = 1;
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

void led_toggle(uint32_t led)
{
  NRF_GPIO->OUT ^= led;
}

void led_turn_off(uint32_t led)
{
  NRF_GPIO->OUT |= led;
}

void led_turn_on(uint32_t led)
{
  NRF_GPIO->OUT &= ~led;
}

void TIMER0_IRQHandler(void)
{
  led_blink_all();
  NRF_TIMER0->EVENTS_COMPARE[0] = 0;//manually clear the event flag
}
