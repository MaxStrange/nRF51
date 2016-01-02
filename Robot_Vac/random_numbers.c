#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"

#include "random_numbers.h"

/*
Blocks until a random number is generated. Then returns
that number. Value is between 0 and 255 inclusive on both
ends.
*/
uint8_t random_numbers_get(void)
{
  NRF_RNG->TASKS_START = 1;

  while (NRF_RNG->EVENTS_VALRDY != 1)
      ;//no-op while you wait for a new random value

  uint8_t value = NRF_RNG->VALUE;

  NRF_RNG->EVENTS_VALRDY = 0;//manually clear the event flag
  NRF_RNG->TASKS_STOP = 1;

  return value;
}
