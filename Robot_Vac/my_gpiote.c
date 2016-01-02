#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "boards.h"
#include "nrf_delay.h"

#include "my_gpiote.h"

static func_ptr button_select_func = NULL;
static func_ptr button_confirm_func = NULL;
static func_ptr range_finder_func = NULL;

void GPIOTE_IRQHandler(void)
{
  if (NRF_GPIOTE->EVENTS_IN[GPIOTE_CHANNEL_SELECT])
  {
    (*button_select_func)();
  }
  else if (NRF_GPIOTE->EVENTS_IN[GPIOTE_CHANNEL_CONFIRM])
  {
    (*button_confirm_func)();
  }
  else if (NRF_GPIOTE->EVENTS_IN[GPIOTE_CHANNEL_RANGE_FINDER])
  {
    (*range_finder_func)();
  }

  NRF_GPIOTE->EVENTS_IN[GPIOTE_CHANNEL_CONFIRM] = 0;//clear the event flag
  NRF_GPIOTE->EVENTS_IN[GPIOTE_CHANNEL_SELECT] = 0;//clear the event flag
  NRF_GPIOTE->EVENTS_IN[GPIOTE_CHANNEL_RANGE_FINDER] = 0;//clear the event flag
}


void gpiote_set_button_select_func(func_ptr p)
{
  button_select_func = p;
}
void gpiote_set_button_confirm_func(func_ptr p)
{
  button_confirm_func = p;
}
void gpiote_set_rf_func(func_ptr p)
{
  range_finder_func = p;
}
