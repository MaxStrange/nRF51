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

// static char buffer[33];//lcd is 16x2 plus one for the nul
// static char user_in[6];//array to take user's input for the timer

/*
BUTTON stuff, not for final version
*/
#define _BUTTON_PIN_NUMBER 16
#define BUTTON (1 << 16)
void GPIOTE_IRQHandler(void);

int main(void)
{
    clock_init();
    led_init();
    uart_init();
    lcd_init();
    lcd_begin();



    /*
    BUTTON stuff, not for final version
    */
    NRF_GPIO->PIN_CNF[_BUTTON_PIN_NUMBER] = (1 << 2);//enable pulldown - button is pulled low until pushed
    NRF_GPIOTE->CONFIG[0] = (1 << 0) | (_BUTTON_PIN_NUMBER << 8)
                              | (2 << 16);//event mode/low to high
    NRF_GPIOTE->INTENSET = 1;//enable interrupt on button's gpiote channel
    NRF_GPIOTE->EVENTS_IN[0] = 0;//clear the event flag manually

    NVIC_ClearPendingIRQ(GPIOTE_IRQn);
    NVIC_SetPriority(GPIOTE_IRQn, 3);
    NVIC_EnableIRQ(GPIOTE_IRQn);
    /*
    End button stuff
    */



    while (true)
    {
      // bool worked = console_go();
      // if (!worked)
      // {
      //   const char *help_str = console_get_help_str();
      //   uart_write_str(help_str);
      //   uart_write_str(" ");
      // }
    }
}

void GPIOTE_IRQHandler(void)
{
  led_blink_all();

  NRF_GPIOTE->EVENTS_IN[0] = 0;//clear the event flag
}
