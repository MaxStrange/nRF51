#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "boards.h"
#include "nrf_delay.h"

#include "random_numbers.h"

#include "drive_system.h"


static e_drive_mode_t current_mode = SPIRAL;
static uint32_t time_started = 0;
static uint32_t current_time = 1;
static uint8_t direction_to_go_to = 90;//if randomizing, we need to go to this direction
static uint8_t current_direction = 90;//direction we are facing

static void drive(void);
static void drive_spiral(void);
static void drive_reverse(void);
static void drive_straight(void);
static void drive_rotate(void);

static void left_motor_stop(void);
static void right_motor_stop(void);
static void left_motor_drive(e_direction_t direction);
static void right_motor_drive(e_direction_t direction);



void drive_system_init(void)
{
  NRF_GPIO->PIN_CNF[_LEFT_MOTOR_FRWRD_PIN_NUMBER] = (1 << 0);//configure as output
  NRF_GPIO->PIN_CNF[_LEFT_MOTOR_REVERSE_PIN_NUMBER] = (1 << 0);//configure as output
  NRF_GPIO->PIN_CNF[_RIGHT_MOTOR_FRWRD_PIN_NUMBER] = (1 << 0);//configure as output
  NRF_GPIO->PIN_CNF[_RIGHT_MOTOR_REVERSE_PIN_NUMBER] = (1 << 0);//configure as output
  drive_system_stop();
}

void drive_system_randomize_direction(uint32_t time_started)
{
  uint8_t random_number = random_numbers_get();
  direction_to_go_to = random_number;
  drive_system_set_mode(RANDOM, time_started);
}

/*
Sets the current mode for driving. Then begins driving in
that mode.
*/
void drive_system_set_mode(e_drive_mode_t mode, uint32_t started)
{
  current_mode = mode;
  time_started = started;
  drive();
}

void drive_system_stop(void)
{
  right_motor_stop();
  left_motor_stop();
}

/*
The drive system only drives using its current mode for a number
of seconds depending on the mode. So you must update it every second
or it will drive indefinitely according to its current mode.
*/
void drive_system_update_time(uint32_t new_time)
{
  current_time = new_time;
  drive();//update the current driving mode with the new time
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
    case RANDOM:
      drive_rotate();
    default:
      //error
      break;
  }
}

static void drive_spiral(void)
{
  //spiral drive happens indefinitely
  right_motor_drive(FORWARDS);
  left_motor_stop();
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
    right_motor_drive(BACKWARDS);
    left_motor_drive(BACKWARDS);
  }
}

static void drive_straight(void)
{
  //straight drive happens indefinitely
  right_motor_drive(FORWARDS);
  left_motor_drive(FORWARDS);
}

static void drive_rotate(void)
{
  //attempts to rotate towards the current specified location
  //once it has reached it, it changes the mode to straight

  if (current_direction == 90)
  {
    drive_system_set_mode(STRAIGHT, current_time);
  }
  else if (direction_to_go_to > 90)
  {
    //turn right
    right_motor_drive(BACKWARDS);
    left_motor_drive(FORWARDS);

    current_direction = (current_direction < 5) ? 0 : (current_direction - 5);
  }
  else
  {
    //turn left
    right_motor_drive(FORWARDS);
    left_motor_drive(BACKWARDS);

    current_direction = (current_direction >= 250) ? 255 : (current_direction + 5);
  }
}

static void left_motor_stop(void)
{
  NRF_GPIO->OUT |= (LEFT_MOTOR_FWRD | LEFT_MOTOR_REVERSE);
}

static void right_motor_stop(void)
{
  NRF_GPIO->OUT |= (RIGHT_MOTOR_FWRD | RIGHT_MOTOR_REVERSE);
}

static void left_motor_drive(e_direction_t direction)
{
  switch (direction)
  {
    case BACKWARDS:
      NRF_GPIO->OUT |= LEFT_MOTOR_FWRD;
      NRF_GPIO->OUT &= ~LEFT_MOTOR_REVERSE;//active low
      break;
    case FORWARDS:
      NRF_GPIO->OUT |= LEFT_MOTOR_REVERSE;
      NRF_GPIO->OUT &= ~LEFT_MOTOR_FWRD;//active low
      break;
  }
}

static void right_motor_drive(e_direction_t direction)
{
  switch (direction)
  {
    case BACKWARDS:
      NRF_GPIO->OUT |= RIGHT_MOTOR_FWRD;
      NRF_GPIO->OUT &= ~RIGHT_MOTOR_REVERSE;//active low
      break;
    case FORWARDS:
      NRF_GPIO->OUT |= RIGHT_MOTOR_REVERSE;
      NRF_GPIO->OUT &= ~RIGHT_MOTOR_FWRD;//active low
      break;
  }
}




/*
Debug functions
*/

void debug_drive_system_backward(void)
{
  left_motor_drive(BACKWARDS);
  right_motor_drive(BACKWARDS);
}

void debug_drive_system_forward(void)
{
  left_motor_drive(FORWARDS);
  right_motor_drive(FORWARDS);
}

void debug_drive_system_random(void)
{
  uint8_t random_number = random_numbers_get();
  direction_to_go_to = random_number;

  while (current_mode != STRAIGHT)
  {
    drive_rotate();
  }

  debug_drive_system_forward();
}

void debug_drive_system_spiral(void)
{
  drive_spiral();
}
