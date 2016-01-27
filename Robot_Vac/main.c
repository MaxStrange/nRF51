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
#include "user_input.h"
#include "random_numbers.h"
#include "drive_system.h"
#include "servo.h"
#include "bumper.h"


static volatile bool flag_bumped_into_something = false;


static uint32_t get_seconds_to_clean_room(e_room_size_t room_size);



static uint32_t get_seconds_to_clean_room(e_room_size_t room_size)
{
  uint32_t seconds_to_clean_room = 0;
  switch (room_size)
  {
    case SMALL:
      seconds_to_clean_room = 300;//5 minutes
      break;
    case MEDIUM:
      seconds_to_clean_room = 900;//15 minutes
      break;
    case LARGE:
      seconds_to_clean_room = 1800;//30 minutes
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

  return seconds_to_clean_room;
}


int main(void)
{
    /*
    Initialize all modules.
    */
    clock_init();
    led_init();
    uart_init();
    lcd_init();
    lcd_begin();
    range_finder_init();
    drive_system_init();
    user_input_init();
    bumper_init(&flag_bumped_into_something);


//    debug console

    while (true)
    {
      bool worked = console_go();
      if (!worked)
      {
        const char *help_str = console_get_help_str();
        uart_write_str(help_str);
        uart_write_str(" ");
      }
    }

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
      When user pushes button, start up the LCD and interact with them until
      you have the data you need.
      */
      e_room_size_t room_size = user_input_get_room_size();
      uint32_t seconds_to_clean_room = get_seconds_to_clean_room(room_size);

      /*
      Set up the timer.
      */
      timer_count_to(seconds_to_clean_room);
      uint16_t seconds_left = seconds_to_clean_room;
      uint16_t last_read_seconds_left = seconds_left;


      drive_system_set_mode(SPIRAL, 0);//start spiral mode at time = 0.

      while (seconds_left > 0)
      {
        seconds_left = timer_get_seconds_left();
        uint16_t seconds_since_start = seconds_to_clean_room - seconds_left;

        /*
        Update the LCD once a second
        */
        if (seconds_left != last_read_seconds_left)
        {
          lcd_clear_and_write("Time left: ");
          lcd_goto(0, 1);
          lcd_write_time(seconds_left);
        }

        /*
        Every so often, randomize the direction and drive off.
        This should help prevent getting stuck on something
        that has for some reason not triggered the bumpers.
        */
        if ((seconds_left % 45) == 0)
        {
          uint16_t time_started_random_mode = seconds_since_start;
          drive_system_randomize_direction(time_started_random_mode);
        }

        /*
        If you bumped into something, reverse.
        */
        if (flag_bumped_into_something)
        {
          uint16_t time_started_reverse_mode = seconds_since_start;
          drive_system_set_mode(REVERSE, time_started_reverse_mode);
          flag_bumped_into_something = false;
        }

        /*
        If the range sensor stops seeing the ground at the normal distance,
        it is possibly a cliff. Back up and try again.
        */
        if (range_finder_get_sees_cliff())
        {
          uint16_t time_started_reverse_mode = seconds_since_start;
          drive_system_set_mode(REVERSE, time_started_reverse_mode);
        }

        drive_system_update_time(seconds_since_start);
        last_read_seconds_left = seconds_left;
      }
      //after timer is done, stop driving
      drive_system_stop();
    }
}
