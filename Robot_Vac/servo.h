#ifndef _SERVO_H_
#define _SERVO_H_

#define _LEFT_SERVO_PIN_NUMBER  25
#define _RIGHT_SERVO_PIN_NUMBER 28

#define LEFT_SERVO  (1 << _LEFT_SERVO_PIN_NUMBER)
#define RIGHT_SERVO (1 << _RIGHT_SERVO_PIN_NUMBER)


void servo_init(void);
void servo_left_goto(uint8_t pos);
void servo_right_goto(uint8_t pos);
void servo_rotate_through_all_angles(void);

void TIMER2_IRQHandler(void);


#endif //_SERVO_H_
