#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "my_uart.h"
#include "nrf.h"


static bool uart_is_on = false;



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
  NRF_GPIO->OUTSET |= TXPIN;  //set the tx pin to high for some reason

  NRF_GPIO->TXPINCFG |= 1; //set tx dir to out
  NRF_GPIO->RXPINCFG &= ~(1 << 1);  //connect rx to input buffer
  NRF_GPIO->RXPINCFG |= (1 << 2) | (1 << 3); //enable the pull-up on rx

  NRF_UART0->PSELTXD |= TXPIN;
  NRF_UART0->PSELRXD |= RXPIN;

  /*
  Configure the baud rate.
  */
  NRF_UART0->BAUDRATE = 0x00275000; //set to 9600 baud

  /*
  Default settings for parity and flow control are fine (no partiy bit,
  no flow control)
  */

  /*
  Ignore interrupts for now
  */
}

void uart_write_str(const char *str)
{
  //if the tx is not on, turn it on
  if (!uart_is_on)
  {
    NRF_UART0->TASKS_STARTTX = 1;
    NRF_UART0->TASKS_STARTRX = 1;
    uart_is_on = true;
  }

  while (*str != '\0')
  {
    //load the next char into the tx buffer
    NRF_UART0->TXD = *str++;

    while (NRF_UART0->EVENTS_TXDRDY == 0)
      ;//block until ready
  }
}
