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
      e_room_size_t room_size = user_input_get_room_size();
      uint32_t seconds_to_clean_room = 0;
      switch (room_size)
      {
        case SMALL:
          seconds_to_clean_room = 20;//TODO : test : 600;//10 minutes
          break;
        case MEDIUM:
          seconds_to_clean_room = 45;//TODO : test : 1800;//30 minutes
          break;
        case LARGE:
          seconds_to_clean_room = 60;//TODO : test : 2700;//45 minutes
          break;
        default:
          //something has gone wrong
          while (true)
          {
            lcd_clear_and_write("Error has occurred");
            uart_write_str("Error has occurred in main: room_size");
            led_pattern_spiral();
            nrf_delay_ms(3000);
          }
      }

      timer_count_to(seconds_to_clean_room);
      uint16_t seconds_left = seconds_to_clean_room;
      uint16_t last_read_seconds_left = seconds_left;
      while (seconds_left > 0)
      {
        seconds_left = timer_get_seconds_left();

        if (seconds_left != last_read_seconds_left)
        {
          lcd_clear_and_write("Time left: ");
          lcd_goto(0, 1);
          lcd_write_int(seconds_left);
        }

        last_read_seconds_left = seconds_left;
      }

      while (true)
      {
        led_pattern_spiral();
        nrf_delay_ms(2000);
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
