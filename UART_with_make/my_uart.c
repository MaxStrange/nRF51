#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "my_uart.h"
#include "nrf.h"
#include "nrf_gpio.h"


void uart_init(void)
{
//configure output
 NRF_GPIO->TXPINCFG |= 1;    //set tx dir to out
 NRF_GPIO->OUTSET = TXPIN;  //set the tx pin to high (see p.151 of ref man)

//configure input
//nrf_gpio_cfg_input(__RX_PIN_NUMBER, NRF_GPIO_PIN_PULLUP);
NRF_GPIO->RXPINCFG &= ~(1 << 1);  //connect rx to input buffer
NRF_GPIO->RXPINCFG |= (1 << 2) | (1 << 3); //enable the pull-up on rx

//PSELTXD
NRF_UART0->PSELTXD = __TX_PIN_NUMBER;//TXPIN;  //tell the uart module which pin is tx

//PSELRXD
NRF_UART0->PSELRXD = __RX_PIN_NUMBER;//RXPIN;  //tell the uart module which pin is rx

//hardware flow control (if enabled)
NRF_UART0->PSELCTS = UART_PIN_DISCONNECTED;
NRF_UART0->PSELRTS = UART_PIN_DISCONNECTED;

//uart interrupts
// NRF_UART0->INTEN = (1 << 7) | (1 << 2);//txdrdy and rxdrdy events trigger IRQs
// NRF_UART0->ERRORSRC = 0x0F;//clear all the all the error flags

//BAUDRATE
NRF_UART0->BAUDRATE = 0x00275000; //set to 9600 baud

//enable
NRF_UART0->ENABLE = 0x04;//enable the uart

//startrx
NRF_UART0->TASKS_STARTRX = 1;

//clear ready events
NRF_UART0->EVENTS_RXDRDY = 0;
NRF_UART0->EVENTS_TXDRDY = 0;
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
