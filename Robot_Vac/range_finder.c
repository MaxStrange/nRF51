#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "boards.h"
#include "nrf_delay.h"

#include "range_finder.h"

static volatile bool pulse_has_not_returned = false;

uint8_t range_finder_get_distance(void)
{
  if (pulse_has_not_returned)
  {
    //there is an erroneous pulse floating around there somewhere,
    //wait for it to echo back by just skipping this method this time
    pulse_has_not_returned = false;

    nrf_delay_ms(5);

    return 0;
  }

  /*
  Enable echo interrupt
  */
  NRF_GPIOTE->EVENTS_IN[GPIOTE_CHANNEL] = 0;//clear the event flag
  NVIC_ClearPendingIRQ(GPIOTE_IRQn);
  NVIC_SetPriority(GPIOTE_IRQn, 3);
  NVIC_EnableIRQ(GPIOTE_IRQn);

  uint16_t microsecs_duration = 0;
  static const uint16_t maximum = 11640;

  /*
  Pulse trigger high
  */
  NRF_GPIO->OUT &= ~TRIGGER;
  nrf_delay_us(2);
  NRF_GPIO->OUT |= TRIGGER;
  nrf_delay_us(15);
  NRF_GPIO->OUT &= ~TRIGGER;

  pulse_has_not_returned = true;
  while ((pulse_has_not_returned) && (microsecs_duration < maximum))
  {
    nrf_delay_us(10);
    microsecs_duration += 10;
  }

  /*
  Return the valid value
  */
  float distance_to_nearest_object = ((float)microsecs_duration / 2.0) / 29.1;
  uint8_t distance_as_int = (uint8_t)(distance_to_nearest_object + 0.5);

  return distance_as_int;
}

void range_finder_init(void)
{
  NRF_GPIO->PIN_CNF[_ECHO_PIN_NUMBER] = (0 << 0) | (1 << 2);//input, pulldown
  NRF_GPIO->PIN_CNF[_TRIGGER_PIN_NUMBER] = (1 << 0);//output
  NRF_GPIO->OUT &= ~TRIGGER;//set trigger to low initially

  /*
  Configure interrupt for the echo pin, but don't enable yet.
  */
  NRF_GPIOTE->CONFIG[GPIOTE_CHANNEL] = (1 << 0) | (_ECHO_PIN_NUMBER << 8) | (2 << 16);//event mode, hi to low
  NRF_GPIOTE->INTENSET = (1 << GPIOTE_CHANNEL);
}



void GPIOTE_IRQHandler(void)
{
  pulse_has_not_returned = false;

  NRF_GPIOTE->EVENTS_IN[GPIOTE_CHANNEL] = 0;//clear the event flag
}
