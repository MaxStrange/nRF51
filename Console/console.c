#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"

#include "my_led.h"
#include "my_uart.h"
#include "my_strings.h"

#include "console.h"

static bool execute_command(const char *cmd);

static const char *help_str = "Usage: blink, dance.\n";
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
    "END_OF_TABLE",
    0x00,
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
  uart_write_str("You just typed: ");
  uart_write_str(console_input);
  uart_write_str("\n");
  nrf_delay_ms(500);//give time for the uart to output

  bool worked = execute_command(console_input);

  return worked;
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
      function_pointer p = allowable_commands[i].execute;
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
