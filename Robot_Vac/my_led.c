#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "boards.h"
#include "nrf_delay.h"

#include "my_led.h"


static void pwm_go(bool high, uint32_t duration_ms);


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

void led_dim_high(uint32_t duration_ms)
{
  bool high = true;
  pwm_go(high, duration_ms);
}

void led_dim_low(uint32_t duration_ms)
{
  bool high = false;
  pwm_go(high, duration_ms);
}

void led_init(void)
{
  NRF_GPIO->DIR |= LED1 | LED2 | LED3 | LED4;
}

void led_pattern_breathe(void)
{
  led_dim_low(1000);
  led_dim_high(1000);
  led_dim_low(1000);
  led_dim_high(1000);
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


static void pwm_go(bool high, uint32_t duration_ms)
{
  /*
  Set up the timer

  Bitmode 0, PRESCALER 9, with cc = 500 is the absolute dimmest you can get it without
  it looking obviously like it is blinking.

  */

  NRF_TIMER0->BITMODE = 0;//16 bit timer  0 : 16bit; 1 : 8bit; 2 : 24; 3 : 32
  NRF_TIMER0->CC[0] = high ? 250 : 400;
  NRF_TIMER0->PRESCALER = 9;
  NRF_TIMER0->EVENTS_COMPARE[0] = 0;//manually clear the event flag

  /*
  Equation: 16,000,000 / (2^PRESCALER) = f_timer
  Duty cycle = time on / (time on + time off) =
      TODO : figure out the math
  */

  /*
  Set up GPIOTE for LEDs.
  */

  static const uint32_t MODE = 3;//task
  static const uint32_t POLARITY = 3;//toggle the led on trigger of event

  uint32_t OUTINIT = (NRF_GPIO->OUT & LED1);
  NRF_GPIOTE->CONFIG[0] = (MODE << 0) | (_LED1_PIN_NUMBER << 8) |
                          (OUTINIT << 20) | (POLARITY << 16);

  OUTINIT = (NRF_GPIO->OUT & LED2);
  NRF_GPIOTE->CONFIG[1] = (MODE << 0) | (_LED2_PIN_NUMBER << 8) |
                          (OUTINIT << 20) | (POLARITY << 16);

  OUTINIT = (NRF_GPIO->OUT & LED3);
  NRF_GPIOTE->CONFIG[2] = (MODE << 0) | (_LED3_PIN_NUMBER << 8) |
                          (OUTINIT << 20) | (POLARITY << 16);

  OUTINIT = (NRF_GPIO->OUT & LED4);
  NRF_GPIOTE->CONFIG[3] = (MODE << 0) | (_LED4_PIN_NUMBER << 8) |
                          (OUTINIT << 20) | (POLARITY << 16);


  /*
  Use PPI to hook up the timer0 module to the LEDs directly.
  */

  //LED1
  NRF_PPI->CH[0].EEP = 0x40008140;//NRF_TIMER0->EVENTS_COMPARE[0];
  NRF_PPI->CH[0].TEP = 0x40006000;//NRF_GPIOTE->TASKS_OUT[0];
  NRF_PPI->CHEN |= (1 << 0);//enable ppi channel 0

  //LED2
  NRF_PPI->CH[2].EEP = 0x40008140;//NRF_TIMER0->EVENTS_COMPARE[0];
  NRF_PPI->CH[2].TEP = 0x40006004;//NRF_GPIOTE->TASKS_OUT[1];
  NRF_PPI->CHEN |= (1 << 2);//enable ppi channel 2

  //LED3
  NRF_PPI->CH[4].EEP = 0x40008140;//NRF_TIMER0->EVENTS_COMPARE[0];
  NRF_PPI->CH[4].TEP = 0x40006008;//NRF_GPIOTE->TASKS_OUT[2];
  NRF_PPI->CHEN |= (1 << 4);//enable ppi channel 4

  //LED4
  NRF_PPI->CH[6].EEP = 0x40008140;//NRF_TIMER0->EVENTS_COMPARE[0];
  NRF_PPI->CH[6].TEP = 0x4000600C;//NRF_GPIOTE->TASKS_OUT[3];
  NRF_PPI->CHEN |= (1 << 6);//enable ppi channel 6

  //also need to clear the timer:
  NRF_PPI->CH[1].EEP = 0x40008140;//NRF_TIMER0->EVENTS_COMPARE[0];
  NRF_PPI->CH[1].TEP = 0x4000800C;//NRF_TIMER0->TASKS_CLEAR;
  NRF_PPI->CHEN |= (1 << 1);//enable ppi channel 1

  /*
  Do the actual pattern
  */

  NRF_TIMER0->TASKS_START = 1;
  nrf_delay_ms(duration_ms);    //display pattern for however long
  NRF_TIMER0->TASKS_STOP = 1;

  /*
  Return to a reasonable state before returning.
  */

  uint8_t i = 0;
  for (i = 0; i < 4; i++)
  {
    NRF_GPIOTE->CONFIG[i] = 0;//disable the gpiote for each led
  }
}
