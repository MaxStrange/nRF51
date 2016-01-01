#ifndef _RANGE_FINDER_H_
#define _RANGE_FINDER_H_

#define _ECHO_PIN_NUMBER 13
#define _TRIGGER_PIN_NUMBER 12

#define ECHO    (1 << _ECHO_PIN_NUMBER)
#define TRIGGER (1 << _TRIGGER_PIN_NUMBER)

#define GPIOTE_CHANNEL 3//The channel in gpiote that this module uses


void GPIOTE_IRQHandler(void);


uint8_t range_finder_get_distance(void);
void range_finder_init(void);




#endif //_RANGE_FINDER_H_
