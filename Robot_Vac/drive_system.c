#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "boards.h"
#include "nrf_delay.h"

#include "servo.h"

#include "drive_system.h"




void drive_system_init(void)
{
  servo_init();
}

void drive_system_randomize_direction(uint32_t time)
{
}

void drive_system_set_mode(e_drive_mode_t mode, uint32_t time)
{
}

void drive_system_update_time(uint32_t time)
{
}
