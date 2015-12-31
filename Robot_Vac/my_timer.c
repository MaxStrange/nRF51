#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_gpio.h"

#include "my_lcd.h"

#include "my_timer.h"

static uint32_t seconds_to_count_to = 0;
static volatile uint32_t seconds_left = 0;
//TODO : should pass in a call back function to use to enter text on the lcd

uint32_t timer_get_seconds_left(void)
{
  return seconds_left;
}

void timer_count_to(uint32_t seconds)
{
  seconds_to_count_to = seconds;
  seconds_left = seconds;

  NRF_RTC0->PRESCALER = 32767;//see Real Time Counter section of ref man//1 Hz
  NRF_RTC0->INTENSET = 1;//enable interrupt on tick event

  NVIC_ClearPendingIRQ(RTC0_IRQn);
  NVIC_SetPriority(RTC0_IRQn, 5);
  NVIC_EnableIRQ(RTC0_IRQn);

  NRF_RTC0->TASKS_START = 1;//start the rtc
}



void RTC_IRQHandler(void)
{
  if (seconds_left > 0)
  {
    seconds_left -= 1;
    lcd_write_int(seconds_left);
  }
  else
  {
    NRF_RTC0->TASKS_STOP = 1;//timer is done
  }

  //either way, clear the event flag
  NRF_RTC0->EVENTS_TICK = 0;
}
