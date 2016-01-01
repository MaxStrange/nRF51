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
#include "servo.h"

// static char buffer[33];//lcd is 16x2 plus one for the nul
// static char user_in[6];//array to take user's input for the timer

int main(void)
{
    clock_init();
    led_init();
    uart_init();
    lcd_init();
    lcd_begin();
    range_finder_init();
    servo_init();

    while (true)
    {
      // bool worked = console_go();
      // if (!worked)
      // {
      //   const char *help_str = console_get_help_str();
      //   uart_write_str(help_str);
      //   uart_write_str(" ");
      // }
      for (uint8_t pos = 0; pos <= 180; pos++)
      {
        servo_left_goto(pos);
      }
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
