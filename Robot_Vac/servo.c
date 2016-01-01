#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "boards.h"
#include "nrf_delay.h"

#include "servo.h"

static void pwm_go(float duty_cycle, uint32_t duration_ms);

void servo_init(void)
{
  NRF_GPIO->PIN_CNF[_SERVO_PIN_NUMBER] = (1 << 0);//configure as output
  NRF_GPIO->OUT &= ~SERVO;
}

/*
Takes an integer between 0 and ?????
*/
void servo_left_goto(uint16_t pos)
{
  /*
  Map:
  0.1     -> 2ms up, 18ms down
  0.075   -> 1.5ms up, 18.5ms down
  0.05    -> 1ms up, 19ms down

  //TODO : map the pos to the duty cycle
  */

  pwm_go(0.075, 200);
}

void servo_right_goto(uint16_t pos)
{
  //TODO
  servo_left_goto(pos);
}





static void pwm_go(float duty_cycle, uint32_t duration_ms)
{
  /*
  Set up the timer
  */
  static const uint16_t upper_lim = 40500;

  uint32_t on_trigger = (uint32_t)(((-1.0) * ((float)upper_lim * (duty_cycle - 1))) + 0.5);

  NRF_TIMER2->BITMODE = 0;//16 bit timer  0 : 16bit; 1 : 8bit; 2 : 24; 3 : 32
  NRF_TIMER2->PRESCALER = 3;

  NRF_TIMER2->CC[0] = on_trigger;
  NRF_TIMER2->CC[1] = upper_lim;

  NRF_TIMER2->INTENSET = (1 << 17) | (1 << 16);//interrupt on cc[1] and cc[0]

  NRF_TIMER2->EVENTS_COMPARE[0] = 0;//manually clear the event flag
  NRF_TIMER2->EVENTS_COMPARE[1] = 0;//manually clear the event flag

  NVIC_ClearPendingIRQ(TIMER2_IRQn);
  NVIC_SetPriority(TIMER2_IRQn, 3);
  NVIC_EnableIRQ(TIMER2_IRQn);

  NRF_TIMER2->TASKS_START = 1;
  nrf_delay_ms(duration_ms);
  NRF_TIMER2->TASKS_STOP = 1;
}


void TIMER2_IRQHandler(void)
{
  if (NRF_TIMER2->EVENTS_COMPARE[0] == 1)
  {
    NRF_GPIO->OUT |= SERVO;
  }
  else if (NRF_TIMER2->EVENTS_COMPARE[1] == 1)
  {
    NRF_GPIO->OUT &= ~SERVO;

    NRF_TIMER2->TASKS_CLEAR = 1;
  }

  NRF_TIMER2->EVENTS_COMPARE[0] = 0;
  NRF_TIMER2->EVENTS_COMPARE[1] = 0;
}
