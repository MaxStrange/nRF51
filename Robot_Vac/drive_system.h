#ifndef _DRIVE_SYSTEM_H_
#define _DRIVE_SYSTEM_H_

typedef enum
{
  REVERSE,
  SPIRAL,
  STRAIGHT,
  RANDOM
} e_drive_mode_t;

typedef enum
{
  BACKWARDS,
  FORWARDS
} e_direction_t;

#define _LEFT_MOTOR_FRWRD_PIN_NUMBER    25
#define _LEFT_MOTOR_REVERSE_PIN_NUMBER  30
#define _RIGHT_MOTOR_FRWRD_PIN_NUMBER   28
#define _RIGHT_MOTOR_REVERSE_PIN_NUMBER 7

#define LEFT_MOTOR_FWRD                 (1 << _LEFT_MOTOR_FRWRD_PIN_NUMBER)
#define LEFT_MOTOR_REVERSE              (1 << _LEFT_MOTOR_REVERSE_PIN_NUMBER)
#define RIGHT_MOTOR_FWRD                (1 << _RIGHT_MOTOR_FRWRD_PIN_NUMBER)
#define RIGHT_MOTOR_REVERSE             (1 << _RIGHT_MOTOR_REVERSE_PIN_NUMBER)

void drive_system_init(void);
void drive_system_set_mode(e_drive_mode_t mode, uint32_t time_started);
void drive_system_update_time(uint32_t time_started);
void drive_system_randomize_direction(uint32_t new_time);
void drive_system_stop(void);

void debug_drive_system_backward(void);
void debug_drive_system_forward(void);
void debug_drive_system_random(void);
void debug_drive_system_spiral(void);

#endif //_DRIVE_SYSTEM_H_
