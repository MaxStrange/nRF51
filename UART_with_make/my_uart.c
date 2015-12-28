#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "my_uart.h"
#include "nrf.h"


void uart_init(void)
{
//code from my dad

//configure output
NRF_GPIO->TXPINCFG |= 1;    //set tx dir to out
NRF_GPIO->OUTSET |= TXPIN;  //set the tx pin to high (see p.151 of ref man)

//configure input
NRF_GPIO->RXPINCFG &= ~(1 << 1);  //connect rx to input buffer
NRF_GPIO->RXPINCFG |= (1 << 2) | (1 << 3); //enable the pull-up on rx

//PSELTXD
NRF_UART0->PSELTXD = TXPIN;  //tell the uart module which pin is tx

//PSELRXD
NRF_UART0->PSELRXD = RXPIN;  //tell the uart module which pin is rx

//hardware flow control (if enabled)
NRF_UART0->PSELCTS = UART_PIN_DISCONNECTED;
NRF_UART0->PSELRTS = UART_PIN_DISCONNECTED;

//uart interrupts
//TODO

//BAUDRATE
NRF_UART0->BAUDRATE = 0x00275000; //set to 9600 baud

//enable
NRF_UART0->ENABLE = 0x04;//enable the uart

//startrx
NRF_UART0->TASKS_STARTRX = 1;

//clear ready events
NRF_UART0->EVENTS_RXDRDY = 0;
NRF_UART0->EVENTS_TXDRDY = 0;

//end code from my dad






#if 0
  /*
  Configure the UART pins.

  On this kit, the RX pin is P0.11 and
  the TX pin is P0.09.
  */
  NRF_UART0->PSELTXD = TXPIN;  //tell the uart module which pin is tx
  NRF_UART0->PSELRXD = RXPIN;  //tell the uart module which pin is rx

  NRF_GPIO->TXPINCFG |= 1;    //set tx dir to out
  NRF_GPIO->OUTSET |= TXPIN;  //set the tx pin to high (see p.151 of ref man)

  NRF_GPIO->RXPINCFG &= ~(1 << 1);  //connect rx to input buffer
  NRF_GPIO->RXPINCFG |= (1 << 2) | (1 << 3); //enable the pull-up on rx
  /*
  Configure the baud rate.
  */
  NRF_UART0->BAUDRATE = 0x00275000; //set to 9600 baud

  /*
  Default settings for parity and flow control are fine (no partiy bit,
  no flow control)
  */

  /*
  Set up the ready bits, disconnect CTS and RTS, and enable the UART

  See p. 38 of ref man for explanation of what events are.
  IMPORTANT: events are only cleared when software writes a 0 to them.
  */
  NRF_UART0->EVENTS_TXDRDY = 0;
  NRF_UART0->EVENTS_RXDRDY = 0;

  NRF_UART0->PSELCTS = UART_PIN_DISCONNECTED;
  NRF_UART0->PSELRTS = UART_PIN_DISCONNECTED;

  NRF_UART0->ENABLE = 0x04;//enable the uart

  /*
  Start the uart module

  See p.38 of ref man for tasks explanation. You start them by writing a 1
  to them.
  */
  NRF_UART0->TASKS_STARTRX = 1;

  /*
  Ignore interrupts for now
  */
  #endif
}

void uart_write_str(const char *str)
{
  while (*str != '\0')
  {
    NRF_UART0->EVENTS_TXDRDY = 0; //we are no longer ready to send
    //load the next char into the tx buffer
    NRF_UART0->TXD = *str++;
    NRF_UART0->TASKS_STARTTX = 1;
  }
}
