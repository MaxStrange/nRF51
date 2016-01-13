#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"

#include "my_led.h"
#include "my_uart.h"
#include "my_strings.h"
#include "my_lcd.h"
#include "my_timer.h"
#include "random_numbers.h"
#include "servo.h"

#include "console.h"

static void drive_servo_forward(void);
static bool execute_command(const char *cmd);
static void timer_countdown_from_10(void);
static void random_number_get_debug(void);

static const char *help_str = "\nUsage: blink, dance, drive, breathe, countdown, random, servo.\n";
static char console_input[INPUT_LENGTH];

static const command allowable_commands[] =
{
  {
    "blink",
    &led_blink_all,
    "Toggle all the LEDs on/off."
  },

  {
    "dance",
    &led_pattern_dance,
    "Displays a dancing pattern on the LEDs."
  },

  {
    "drive",
    &drive_servo_forward,
    "Drives the servo forward for 10 seconds."
  },

  {
    "breathe",
    &led_pattern_breathe,
    "Displays a breathing pattern on the LEDs."
  },

  {
    "spiral",
    &led_pattern_spiral,
    "Displays a spiral pattern on the LEDs."
  },

  {
    "countdown",
    &timer_countdown_from_10,//not in timer module - purely debug function
    "Counts down on the lcd from 10."
  },

  {
    "servo",
    &servo_rotate_through_all_angles,
    "Rotates the servo through all angles."
  },

  {
    "random",
    &random_number_get_debug,//not in random number module - purely debug function
    "Gets a random number."
  },

  {
    "END_OF_TABLE",
    NULL,
    "End of table indicator."
  }
};

const char * console_get_help_str(void)
{
  return help_str;
}

/*
Attempts to get a command from the user and
execute it.
Returns whether or not it succeeded.
*/
bool console_go(void)
{
  /*
  Reinitialize console string
  */
  uint32_t i = 0;
  for (i = 0; i < INPUT_LENGTH; i++)
  {
    console_input[i] = 0;
  }

  uart_get_console_input(console_input, INPUT_LENGTH);
  uart_write_str("\nYou just typed: ");
  uart_write_str(console_input);
  uart_write_str("\n");
  nrf_delay_ms(500);//give time for the uart to output

  bool worked = execute_command(console_input);

  return worked;
}


static void drive_servo_forward(void)
{
  servo_left_goto(180);
}


static bool execute_command(const char *cmd)
{
  uint32_t i = 0;
  const char *name = allowable_commands[i].name;
  while (!strings_compare(name, "END_OF_TABLE"))
  {
    name = allowable_commands[i].name;
    if (strings_compare(cmd, name))
    {
      func_ptr p = allowable_commands[i].execute;
      (*p)();//executes the command's function pointer
      return true;
    }
    else
    {
      i++;
    }
  }

  return false;//never found the given function
}

static void timer_countdown_from_10(void)
{
  led_all_on();

  timer_count_to(10);

  while (timer_get_seconds_left() > 0)
  {
    lcd_clear();
    lcd_goto(0, 0);
    lcd_write_int(timer_get_seconds_left());
  }

  led_all_off();
}

static void random_number_get_debug(void)
{
  uint8_t r = random_numbers_get();
  uart_write_str("\nHere is your number: ");
  uart_write_int(r);
  uart_write_str(" ");
}
