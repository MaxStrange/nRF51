#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_gpio.h"

#include "my_clock.h"


void clock_init(void)
{
  NRF_CLOCK->TASKS_LFCLKSTOP = 1;
  NRF_CLOCK->LFCLKSRC = 0;//1;//source for lf clock is external crystal
  NRF_CLOCK->TASKS_LFCLKSTART = 1;//start the lf clock

  while ((NRF_CLOCK->LFCLKSTAT & (1 << 16)) == 0)
      ;//wait until the clock has settled
  while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
      ;
}
