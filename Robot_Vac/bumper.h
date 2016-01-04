#ifndef _BUMPER_H_
#define _BUMPER_H_

#define _BUMPER_PIN_NUMBER 16

#define BUMPER (1 << _BUMPER_PIN_NUMBER)



void bumper_init(volatile bool *flag_bumped_into_something);

#endif //_BUMPER_H_
