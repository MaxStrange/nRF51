#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "boards.h"
#include "nrf_delay.h"

#include "servo.h"


static uint8_t left_current_pos = 90;
static uint8_t right_current_pos = 90;

static void pwm_go(bool left, float duty_cycle);


void servo_init(void)
{
  NRF_GPIO->PIN_CNF[_LEFT_SERVO_PIN_NUMBER] = (1 << 0);//configure as output
  NRF_GPIO->PIN_CNF[_RIGHT_SERVO_PIN_NUMBER] = (1 << 0);//configure as output
  NRF_GPIO->OUT &= ~(LEFT_SERVO | RIGHT_SERVO);
}

/*
Takes an integer between 0 and 180. A value of 90 signifies
the servo's normal resting position.
*/
void servo_left_goto(uint8_t pos)
{
  //If the servo is already going in the passed in direction, ignore it.
  if (pos == left_current_pos)
    return;
  else
    left_current_pos = pos;

  if (pos > 180)
    pos = 180;
  /*
  Map:
  0.028 -> max counter clockwise
  0.11  -> max clockwise

  D: 0 : 180 --- R: 0.028 : 0.11
  */


  float location = (pos * (110 - 28) / 180) + 28;
  location /= 1000.0;

  bool left = true;
  pwm_go(left, location);
}

void servo_right_goto(uint8_t pos)
{
  //If the servo is already going in the passed in direction, ignore it.
  if (pos == right_current_pos)
    return;
  else
    right_current_pos = pos;

  if (pos > 180)
    pos = 180;

  float location = (pos * (110 - 28) / 180) + 28;
  location /= 1000.0;

  bool left = false;
  pwm_go(left, location);
}

void servo_rotate_through_all_angles(void)
{
  for (uint8_t i = 0; i <= 180; i++)
  {
    servo_left_goto(i);
  }
  for (uint8_t i = 180; i > 0; i--)
  {
    servo_left_goto(i);
  }
}





static void pwm_go(bool left, float duty_cycle)
{
  NRF_TIMER2->TASKS_STOP = 1;

  /*
  Set up the timer
  */
  static const uint16_t upper_lim = 40500;

  uint32_t on_trigger = (uint32_t)(((-1.0) * ((float)upper_lim * (duty_cycle - 1))) + 0.5);

  NRF_TIMER2->BITMODE = 0;//16 bit timer  0 : 16bit; 1 : 8bit; 2 : 24; 3 : 32
  NRF_TIMER2->PRESCALER = 3;

  uint8_t on_trigger_cc_index = left ? 0 : 2;
  NRF_TIMER2->CC[on_trigger_cc_index] = on_trigger;
  NRF_TIMER2->CC[1] = upper_lim;

  uint8_t on_trigger_bit = left ? 16 : 18;
  NRF_TIMER2->INTENSET = (1 << 17) | (1 << on_trigger_bit);//interrupt on cc[1] and cc[0 or 2]


  /*
  Set up the interrupts for the timer
  */
  NRF_TIMER2->EVENTS_COMPARE[on_trigger_cc_index] = 0;//manually clear the event flag
  NRF_TIMER2->EVENTS_COMPARE[1] = 0;//manually clear the event flag

  NVIC_ClearPendingIRQ(TIMER2_IRQn);
  NVIC_SetPriority(TIMER2_IRQn, 3);
  NVIC_EnableIRQ(TIMER2_IRQn);


  /*
  Do the actual task.
  */
  NRF_TIMER2->TASKS_START = 1;
  // nrf_delay_ms(duration_ms);
  // NRF_TIMER2->TASKS_STOP = 1;
}


void TIMER2_IRQHandler(void)
{
  if (NRF_TIMER2->EVENTS_COMPARE[0] == 1)
  {
    NRF_GPIO->OUT |= LEFT_SERVO;
    NRF_TIMER2->EVENTS_COMPARE[0] = 0;
  }
  else if (NRF_TIMER2->EVENTS_COMPARE[1] == 1)
  {
    NRF_GPIO->OUT &= ~LEFT_SERVO;
    NRF_GPIO->OUT &= ~RIGHT_SERVO;
    NRF_TIMER2->EVENTS_COMPARE[1] = 0;
  }
  else if (NRF_TIMER2->EVENTS_COMPARE[2] == 1)
  {
    NRF_GPIO->OUT |= RIGHT_SERVO;
    NRF_TIMER2->EVENTS_COMPARE[2] = 0;
  }
}
