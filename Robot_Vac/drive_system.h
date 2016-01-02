#ifndef _DRIVE_SYSTEM_H_
#define _DRIVE_SYSTEM_H_

typedef enum
{
  REVERSE,
  SPIRAL,
  STRAIGHT
} e_drive_mode_t;

void drive_system_init(void);
void drive_system_set_mode(e_drive_mode_t mode, uint32_t time_started);
void drive_system_update_time(uint32_t time_started);
void drive_system_randomize_direction(uint32_t new_time);
void drive_system_stop(void);

#endif //_DRIVE_SYSTEM_H_
