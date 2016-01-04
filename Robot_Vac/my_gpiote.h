#ifndef _MY_GPIOTE_H_
#define _MY_GPIOTE_H_

#define GPIOTE_CHANNEL_CONFIRM      0
#define GPIOTE_CHANNEL_SELECT       1
#define GPIOTE_CHANNEL_BUMPER       2
#define GPIOTE_CHANNEL_RANGE_FINDER 3

typedef void(*func_ptr)(void);

void gpiote_set_bumper_func(func_ptr p);
void gpiote_set_button_select_func(func_ptr p);
void gpiote_set_button_confirm_func(func_ptr p);
void gpiote_set_rf_func(func_ptr p);


void GPIOTE_IRQHandler(void);


#endif //_MY_GPIOTE_H_
