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
#include "user_input.h"

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
    user_input_init();

    //debug console

    // while (true)
    // {
    //   bool worked = console_go();
    //   if (!worked)
    //   {
    //     const char *help_str = console_get_help_str();
    //     uart_write_str(help_str);
    //     uart_write_str(" ");
    //   }
    // }

    while (true)
    {
      /*
      Sleep until woken by user input
      TODO : Sleep mode OFF can't be tested directly while hooked up to the
      debugger, since it instead enters EMULATED system off, which actually
      accidentally executes code after it is supposed to be asleep. Who knows
      what other mischief the CPU gets into when it is supposed to be sleeping.
      */

      /*
      When user pushed button, start up the LCD and interact with them until
      you have the data you need.
      */
      //e_room_size_t room_size = user_input_get_room_size();
//      user_input_get_room_size();

      uint8_t dist = range_finder_get_distance();
      uart_write_str("Dist: "); uart_write_int(dist); uart_write_str("\n");
      nrf_delay_ms(1000);
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
