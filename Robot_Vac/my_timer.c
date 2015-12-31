#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#include "my_timer.h"

static const uint32_t PRESCALER_VALUE = 64000;
static const uint8_t TICKS_PER_ROLLOVER = 14;

static volatile uint8_t ticks_so_far_this_second = 0;
static uint32_t seconds_to_count_to = 0;
static volatile uint32_t seconds_left = 0;

uint32_t timer_get_seconds_left(void)
{
  return seconds_left;
}

void timer_count_to(uint32_t seconds)
{
  seconds_to_count_to = seconds;
  seconds_left = seconds;

  /*
  Reset the RTC if it is in use or has been used.
  */
  NRF_RTC0->TASKS_STOP = 1;
  NRF_RTC0->TASKS_CLEAR = 1;
  NRF_RTC0->EVENTS_TICK = 0;//clear the timer flag


  /*
  Configure the RTC
  */
  NRF_RTC0->EVTENSET = 1;//enable event routing on tick event
  NRF_RTC0->PRESCALER = PRESCALER_VALUE;//6.9 Hz - close to a whole number

  NRF_RTC0->INTENSET = (1 << 0);//enable interrupt on tick event and overflow

  /*
  Configre Interrupts
  */
  NVIC_ClearPendingIRQ(RTC0_IRQn);
  NVIC_SetPriority(RTC0_IRQn, 3);
  NVIC_EnableIRQ(RTC0_IRQn);

  /*
  Start up the RTC
  */
  NRF_RTC0->TASKS_START = 1;//start the rtc
}

void RTC0_IRQHandler(void)
{
  if (seconds_left > 0)
  {
    ticks_so_far_this_second += 1;
    if (ticks_so_far_this_second >= TICKS_PER_ROLLOVER)
    {
      ticks_so_far_this_second = 0;
      seconds_left -= 1;
    }
  }
  else
  {
    seconds_left = 0;

    NRF_RTC0->TASKS_STOP = 1;//timer is done
    NRF_RTC0->TASKS_CLEAR = 1;//clear the timer
  }

  //either way, clear the event flag
  NRF_RTC0->EVENTS_TICK = 0;
}
