#ifndef _MY_TIMER_H_
#define _MY_TIMER_H_

void timer_count_to(uint32_t seconds);
uint32_t timer_get_seconds_left(void);

void RTC0_IRQHandler(void);

#endif //_MY_TIMER_H_
