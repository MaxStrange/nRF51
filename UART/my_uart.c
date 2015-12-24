#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "my_uart.h"

void uart_init(void)
{
  /*
  * The example code found in app_uart looks like I need to do
  * the following things in order to initialize the UART:
  */

  //configure rx and tx pins

  //configure baud rate and parity

  //configure flow control

  //(setup the gpiote to handle events) ?

  //enable uart interrupts

  //then you are done!



  /*
  Configure the UART pins.

  On this kit, the RX pin is P0.11 and
  the TX pin is P0.09.
  */
}
