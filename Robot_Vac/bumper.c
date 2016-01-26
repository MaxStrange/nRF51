#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "boards.h"
#include "nrf_delay.h"

#include "my_gpiote.h"

#include "bumper.h"

static volatile bool *bumped_into_something = NULL;


static void bumper_func(void);


void bumper_init(volatile bool * const flag_bumped_into_something)
{
  bumped_into_something = flag_bumped_into_something;


  NRF_GPIO->PIN_CNF[_BUMPER_PIN_NUMBER] = (0 << 1) | (1 << 2) | (2 << 16);//input buffer, pullup, sense high

  /*
  Configure interrupt for the bumper pin
  */
  NRF_GPIOTE->CONFIG[GPIOTE_CHANNEL_BUMPER] = (1 << 0) | (_BUMPER_PIN_NUMBER << 8) | (1 << 16);//event mode, hi to low
  NRF_GPIOTE->INTENSET = (1 << GPIOTE_CHANNEL_BUMPER);

  NRF_GPIOTE->EVENTS_IN[GPIOTE_CHANNEL_BUMPER] = 0;//clear the event flag
  NVIC_ClearPendingIRQ(GPIOTE_IRQn);
  NVIC_SetPriority(GPIOTE_IRQn, 3);
  NVIC_EnableIRQ(GPIOTE_IRQn);
  gpiote_set_bumper_func(&bumper_func);
}



void bumper_func(void)
{
  nrf_delay_us(1500);
  if (!(NRF_GPIO->IN & BUMPER))
    return;//debounce

  (*bumped_into_something) = true;
}
