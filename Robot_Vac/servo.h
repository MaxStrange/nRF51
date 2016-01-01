#ifndef _SERVO_H_
#define _SERVO_H_

#define _SERVO_PIN_NUMBER 25

#define SERVO (1 << _SERVO_PIN_NUMBER)


void servo_init(void);
void servo_left_goto(uint8_t pos);
void servo_right_goto(uint8_t pos);

void TIMER2_IRQHandler(void);


#endif //_SERVO_H_
