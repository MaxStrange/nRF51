#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "boards.h"
#include "nrf_delay.h"

#include "servo.h"
#include "random_numbers.h"

#include "drive_system.h"


static e_drive_mode_t current_mode = SPIRAL;
static uint32_t time_started = 0;
static uint32_t current_time = 1;

static void drive(void);
static void drive_spiral(void);
static void drive_reverse(void);
static void drive_straight(void);

void drive_system_init(void)
{
  servo_init();
}

void drive_system_randomize_direction(uint32_t time_started)
{
  //TODO://uint8_t random_number = random_numbers_get();
  drive_system_set_mode(STRAIGHT, time_started);
  drive();
}

/*
Sets the current mode for driving. Then begins driving in
that mode.
*/
void drive_system_set_mode(e_drive_mode_t mode, uint32_t time_started)
{
  current_mode = mode;

  drive();
}

void drive_system_stop(void)
{
  //TODO
  servo_left_goto(90);
  servo_right_goto(90);
}

/*
The drive system only drives using its current mode for a number
of seconds depending on the mode. So you must update it every second
or it will drive indefinitely according to its current mode.
*/
void drive_system_update_time(uint32_t new_time)
{
  current_time = new_time;
}



static void drive(void)
{
  switch (current_mode)
  {
    case SPIRAL:
      drive_spiral();
      break;
    case STRAIGHT:
      drive_straight();
      break;
    case REVERSE:
      drive_reverse();
      break;
    default:
      //error
      break;
  }
}

static void drive_spiral(void)
{
  //spiral drive happens indefinitely

  //TODO
  servo_left_goto(180 - current_time);
  servo_right_goto(180);
}

static void drive_reverse(void)
{
  static const uint8_t reverse_drive_time = 5; //reverse drive only happens for five seconds
  if ((current_time - time_started) >= reverse_drive_time)
  {
    drive_system_stop();
    drive_system_randomize_direction(current_time);
  }
  else
  {
    //TODO
    servo_left_goto(0);
    servo_right_goto(0);
  }
}

static void drive_straight(void)
{
  //straight drive happens indefinitely

  servo_left_goto(180);
  servo_right_goto(180);
}
