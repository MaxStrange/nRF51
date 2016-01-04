#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "boards.h"
#include "nrf_delay.h"

#include "servo.h"


static volatile bool control_left_servo = true;//whether or not the ISR should use the left servo


static void pwm_go(bool left, float duty_cycle, uint32_t duration_ms);


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
  pwm_go(left, location, 5);
}

void servo_right_goto(uint8_t pos)
{
  if (pos > 180)
    pos = 180;

  float location = (pos * (110 - 28) / 180) + 28;
  location /= 1000.0;

  bool left = false;
  pwm_go(left, location, 5);
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





static void pwm_go(bool left, float duty_cycle, uint32_t duration_ms)
{
  /*
  Set up the flag that tells the ISR what servo to control.
  */
  control_left_servo = left;

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


  /*
  Set up the interrupts for the timer
  */

  NRF_TIMER2->EVENTS_COMPARE[0] = 0;//manually clear the event flag
  NRF_TIMER2->EVENTS_COMPARE[1] = 0;//manually clear the event flag

  NVIC_ClearPendingIRQ(TIMER2_IRQn);
  NVIC_SetPriority(TIMER2_IRQn, 3);
  NVIC_EnableIRQ(TIMER2_IRQn);


  /*
  Do the actual task.
  */
  NRF_TIMER2->TASKS_START = 1;
  nrf_delay_ms(duration_ms);
  NRF_TIMER2->TASKS_STOP = 1;
}


void TIMER2_IRQHandler(void)
{
  uint32_t servo = control_left_servo ? LEFT_SERVO : RIGHT_SERVO;

  if (NRF_TIMER2->EVENTS_COMPARE[0] == 1)
  {
    NRF_GPIO->OUT |= servo;
  }
  else if (NRF_TIMER2->EVENTS_COMPARE[1] == 1)
  {
    NRF_GPIO->OUT &= ~servo;

    NRF_TIMER2->TASKS_CLEAR = 1;
  }

  NRF_TIMER2->EVENTS_COMPARE[0] = 0;
  NRF_TIMER2->EVENTS_COMPARE[1] = 0;
}
