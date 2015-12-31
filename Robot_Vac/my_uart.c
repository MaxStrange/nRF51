#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_gpio.h"

#include "circular_queue.h"
#include "my_led.h"
#include "my_strings.h"

#include "my_uart.h"


/*
Static helper function declarations
*/
static void get_user_input(void);
static void read_from_rx_into_str(char *str, uint32_t str_length);
static void reinitialize_rx_queue(void);

/*
ISR helpers
*/
static inline void read_data_into_rx_buffer(void);
static inline void send_next_byte(void);

/*
Internal state
*/
static char int_buffer[20];//used for writing ints to the uart
static volatile circular_queue rx_buffer;
static volatile circular_queue tx_buffer;
static volatile uint32_t num_chars_from_user = 0;
static volatile bool rx_data_is_complete = false;



/*
Outputs an interface to the console. Then blocks until the user
inputs a response. Then places the response in the buffer with a nul
terminator and returns.
*/
void uart_get_console_input(char *buffer, uint32_t buffer_length)
{
  /*
  Get stuff from the user
  */
  do
  {
    reinitialize_rx_queue();
    uart_write_str("Please enter a command: ");
    get_user_input();

  } while(num_chars_from_user >= buffer_length);
  num_chars_from_user = 0;

  /*
  Put the stuff you got from the user into the string that was
  passed in.
  */
  read_from_rx_into_str(buffer, buffer_length);
}

void uart_init(void)
{
  circular_queue_construct(&tx_buffer);
  circular_queue_construct(&rx_buffer);

 //configure output
 NRF_GPIO->TXPINCFG |= 1;    //set tx dir to out
 NRF_GPIO->OUT |= TXPIN;  //set the tx pin to high (see p.151 of ref man)

//configure input
//nrf_gpio_cfg_input(__RX_PIN_NUMBER, NRF_GPIO_PIN_PULLUP);
NRF_GPIO->RXPINCFG &= ~(1 << 1);  //connect rx to input buffer
NRF_GPIO->RXPINCFG |= (1 << 2) | (1 << 3); //enable the pull-up on rx

//PSELTXD
NRF_UART0->PSELTXD = __TX_PIN_NUMBER;//tell the uart module which pin is tx

//PSELRXD
NRF_UART0->PSELRXD = __RX_PIN_NUMBER;//tell the uart module which pin is rx

//hardware flow control (if enabled)
NRF_UART0->PSELCTS = UART_PIN_DISCONNECTED;
NRF_UART0->PSELRTS = UART_PIN_DISCONNECTED;

//uart interrupts
NRF_UART0->INTENCLR = 0xFFFFFFFFUL;
NRF_UART0->INTENSET = (1 << 7) | (1 << 2);//txdrdy and rxdrdy events trigger IRQs

NVIC_ClearPendingIRQ(UART0_IRQn);
NVIC_SetPriority(UART0_IRQn, 3);
NVIC_EnableIRQ(UART0_IRQn);

//BAUDRATE
NRF_UART0->BAUDRATE = 0x00275000;//set to 9600 baud

//enable
NRF_UART0->ENABLE = 0x04;//enable the uart

//startrx
NRF_UART0->TASKS_STARTRX = 1;

//clear ready events
NRF_UART0->EVENTS_RXDRDY = 0;
NRF_UART0->EVENTS_TXDRDY = 0;
}

void uart_write_int(uint32_t i)
{
  strings_int_to_str(i, int_buffer);
  uart_write_str(int_buffer);
}

void uart_write_str(const char *str)
{
  while (*str != '\0')
  {
    circular_queue_write_char(&tx_buffer, *str++);
  }

  send_next_byte();//have to load a character into the buffer before ISR will fire
  NRF_UART0->TASKS_STARTTX = 1;//let interrupts take care of sending the data
}


/*
Static helper functions
*/


/*
Blocks until the user inputs data that ends with a \n.
Reads each character one at a time into the rx_buffer,
but replaces the last char with a nul terminator.
*/
static void get_user_input(void)
{
  bool rx_interrupt_already_enabled = ((NRF_UART0->INTENSET) & (1 << 7));

  NRF_UART0->INTENSET = (1 << 7);
  while (!rx_data_is_complete)
    ;//block until you get a complete statement from user
  NRF_UART0->INTENCLR = (uint8_t)rx_interrupt_already_enabled;

  rx_data_is_complete = false;
}

static void read_from_rx_into_str(char *str, uint32_t str_length)
{
  uint32_t i = 0;
  for (i = 0; i < str_length; i++)
  {
    str[i] = circular_queue_read_next_char(&rx_buffer);
    if (str[i] == '\n')
      break;
  }
  str[i] = '\0';//replace the last char with the nul.
}

static void reinitialize_rx_queue(void)
{
  uint32_t i = 0;

  for (i = 0; i < num_chars_from_user; i++)
  {
    circular_queue_remove_next_char(&rx_buffer);
  }

  num_chars_from_user = 0;
}



/*
ISRs and related functions
*/



//TODO : HOW DO I HOOK THIS HANDLER UP TO THE ACTUAL INTERRUPT???
void UART0_IRQHandler(void)
{
  if (NRF_UART0->EVENTS_TXDRDY)
  {
    //must clear the event flags yourself
    NRF_UART0->EVENTS_TXDRDY = 0;

    send_next_byte();
  }

  if (NRF_UART0->EVENTS_RXDRDY)
  {
    //must clear event flags yourself
    NRF_UART0->EVENTS_RXDRDY = 0;

    read_data_into_rx_buffer();
  }
}


static inline void read_data_into_rx_buffer(void)
{
  char c = NRF_UART0->RXD;
  circular_queue_write_char(&rx_buffer, c);
  rx_data_is_complete = (c == '\n');
  num_chars_from_user++;
}

static inline void send_next_byte(void)
{
  if (circular_queue_is_empty(&tx_buffer))
  {
    //disable the tx interrupt until there is some more data to send
    NRF_UART0->TASKS_STOPTX = 1;
    return;
  }
  else
  {
    NRF_UART0->TXD = circular_queue_read_next_char(&tx_buffer);
  }
}
