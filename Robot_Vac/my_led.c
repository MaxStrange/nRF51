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

void led_all_on(void)
{
  NRF_GPIO->OUT &= ~(LED1 | LED2 | LED3 | LED4);
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
  led_all_off();

  /*
  Set up the timer
  */
  /*
  Equation: 16,000,000 / (2^PRESCALER) = f_timer
  When PRESCALER is 8, f = 62,500 ticks per second - so about one full
  16 bit width per second. So with these settings, it should trigger
  the capture event about once per second.
  */
  //NRF_TIMER0->MODE = 1;//counter mode
  NRF_TIMER0->BITMODE = 0;//16 bit timer
  NRF_TIMER0->CC[0] = 32000;
//  NRF_TIMER0->INTENSET = (1 << 16);//enable interrupt on compare on chanel 0
  NRF_TIMER0->PRESCALER = 8;
  NRF_TIMER0->EVENTS_COMPARE[0] = 0;//manually clear the event flag

  /*
  Set up GPIOTE for LEDs.
  */

  static const uint32_t MODE = 3;//task
  static const uint32_t PSEL = _LED1_PIN_NUMBER;//the led to to display on
  static const uint32_t POLARITY = 3;//toggle the led on trigger of event
  uint32_t OUTINIT = 1;//(NRF_GPIO->OUT & LED1);
  NRF_GPIOTE->CONFIG[0] = (MODE << 0) | (PSEL << 8) |
                          (OUTINIT << 20) | (POLARITY << 16);

  /*
  Use PPI to hook up the timer0 module to the LEDs directly.
  */

  NRF_PPI->CH[0].EEP = 0x40008140;//NRF_TIMER0->EVENTS_COMPARE[0];
  NRF_PPI->CH[0].TEP = 0x40006000;//NRF_GPIOTE->TASKS_OUT[0];
  NRF_PPI->CHEN |= 1;//enable ppi channel 0
  NRF_PPI->CHG[0] |= 1;//include channel 0 in channel group 0
  NRF_PPI->TASKS_CHG[0].EN = 1;//enable the channel group


  // NVIC_ClearPendingIRQ(TIMER0_IRQn);
  // NVIC_SetPriority(TIMER0_IRQn, 3);
  // NVIC_EnableIRQ(TIMER0_IRQn);

  NRF_TIMER0->TASKS_START = 1;

  nrf_delay_ms(4000);    //display pattern for however long

  NRF_TIMER0->TASKS_STOP = 1;

  NRF_GPIOTE->CONFIG[0] = 0;//disable the gpiote for LED1
  led_all_on();        //leave the LEDs on at the end
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
  //led_blink_all();
  if (NRF_TIMER0->EVENTS_COMPARE[0] == 1)
  {
    led_turn_on(LED2);
    NRF_GPIOTE->TASKS_OUT[0] = 1;
  }
  NRF_TIMER0->EVENTS_COMPARE[0] = 0;//manually clear the event flag
}
