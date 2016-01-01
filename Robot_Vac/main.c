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
#include "my_strings.h"
#include "range_finder.h"

// static char buffer[33];//lcd is 16x2 plus one for the nul
// static char user_in[6];//array to take user's input for the timer

void TIMER1_IRQHandler(void);
static void pwm_go(float duty_cycle, uint32_t duration_ms);


#define _SERVO_PIN_NUMBER 25
#define SERVO (1 << _SERVO_PIN_NUMBER)


static void pwm_go(float duty_cycle, uint32_t duration_ms)
{
  /*
  Set up the timer
  */
  static const uint16_t upper_lim = 40500;

  uint32_t on_trigger = (uint32_t)(((-1.0) * ((float)upper_lim * (duty_cycle - 1))) + 0.5);

  NRF_TIMER1->BITMODE = 0;//16 bit timer  0 : 16bit; 1 : 8bit; 2 : 24; 3 : 32
  NRF_TIMER1->CC[0] = on_trigger;
  NRF_TIMER1->PRESCALER = 3;//2;
  NRF_TIMER1->EVENTS_COMPARE[0] = 0;//manually clear the event flag

  NRF_TIMER1->CC[1] = upper_lim;
  NRF_TIMER1->INTENSET = (1 << 17) | (1 << 16);//interrupt on cc[1] and cc[0]
  NRF_TIMER1->EVENTS_COMPARE[1] = 0;//manually clear the event flag

  NVIC_ClearPendingIRQ(TIMER1_IRQn);
  NVIC_SetPriority(TIMER1_IRQn, 3);
  NVIC_EnableIRQ(TIMER1_IRQn);

  NRF_TIMER1->TASKS_START = 1;
}

void TIMER1_IRQHandler(void)
{
  if (NRF_TIMER1->EVENTS_COMPARE[0] == 1)
  {
    //led_all_on();
    NRF_GPIO->OUT |= SERVO;
  }
  else if (NRF_TIMER1->EVENTS_COMPARE[1] == 1)
  {
    //led_all_off();
    NRF_GPIO->OUT &= ~SERVO;

    NRF_TIMER1->TASKS_CLEAR = 1;
  }

  NRF_TIMER1->EVENTS_COMPARE[0] = 0;
  NRF_TIMER1->EVENTS_COMPARE[1] = 0;
}

int main(void)
{
    clock_init();
    led_init();
    uart_init();
    lcd_init();
    lcd_begin();
    range_finder_init();

    NRF_GPIO->PIN_CNF[_SERVO_PIN_NUMBER] = (1 << 0);//configure as output
    NRF_GPIO->OUT &= ~SERVO;

    while (true)
    {
      // bool worked = console_go();
      // if (!worked)
      // {
      //   const char *help_str = console_get_help_str();
      //   uart_write_str(help_str);
      //   uart_write_str(" ");
      // }
      /*
      Map:
      0.1     -> 2ms up, 18ms down
      0.075   -> 1.5ms up, 18.5ms down
      0.05    -> 1ms up, 19ms down
      */
      pwm_go(0.075, 200);

      // //bit bang the servo
      // nrf_delay_ms(18);
      // NRF_GPIO->OUT |= SERVO;
      // nrf_delay_us(2000);
      // NRF_GPIO->OUT &= ~SERVO;
    }

    /*
    High level pseudo code:

    -Initialize everything

    while (true):
      -sleep
      -wake up on gpio input interrupt
      -get user Data
      -set up timer
      -drive spiral mode on

      while (timer not finished)
        -display timer Data on lcd

        if (drive spiral mode)
          -drive motors in spiral pattern

        if (bumped in to something)
          -back up
          -bumped into something = false
          -drive straight mode on
          -drive spiral mode off
          -turn a random amount

        if (drive straight mode)
          -drive in straight line

        if (range finder does not sense ground)
          -back up
          -turn 90 degrees
          -drive straight mode on
          -drive spiral mode off
      end while timer not finished
    end while true
    */

}
