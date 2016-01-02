#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "boards.h"
#include "nrf_delay.h"

#include "my_lcd.h"
#include "my_gpiote.h"

#include "user_input.h"

static volatile bool user_should_be_selecting = false;
static volatile bool user_has_selected_a_size = false;
static volatile e_room_size_t room_size_to_display = SMALL;
static const char * small_str = "SMALL";
static const char * medium_str = "MEDIUM";
static const char * large_str = "LARGE";
static void button_select_func(void);
static void button_confirm_func(void);


e_room_size_t user_input_get_room_size(void)
{
  user_should_be_selecting = true;
  const char * selected;

  while (!user_has_selected_a_size)
  {
    nrf_delay_ms(100);
    lcd_clear_and_write("Room Size?");
    lcd_goto(0, 1);
    switch (room_size_to_display)
    {
      case SMALL:
        selected = small_str;
        break;
      case MEDIUM:
        selected = medium_str;
        break;
      case LARGE:
        selected = large_str;
        break;
      default:
        selected = "ERROR";
        break;
    }
    lcd_write_str(selected);
  }

  lcd_clear_and_write("You selected: ");
  lcd_goto(0, 1);
  lcd_write_str(selected);
  return room_size_to_display;//the one that is on the display is the one the user selected
}

void user_input_init(void)
{
  NRF_GPIO->PIN_CNF[_CONFIRM_BUTTON_PIN_NUMBER] = (0 << 1) | (1 << 2) | (2 << 16);//input buffer, pullup, sense high
  NRF_GPIO->PIN_CNF[_SELECTION_BUTTON_PIN_NUMBER] = (0 << 1) | (1 << 2) | (2 << 16);

  /*
  Configure interrupt for the button pins
  */
  NRF_GPIOTE->CONFIG[GPIOTE_CHANNEL_CONFIRM] = (1 << 0) | (_CONFIRM_BUTTON_PIN_NUMBER << 8) | (1 << 16);//event mode, hi to low
  NRF_GPIOTE->INTENSET = (1 << GPIOTE_CHANNEL_CONFIRM);

  NRF_GPIOTE->CONFIG[GPIOTE_CHANNEL_SELECT] = (1 << 0) | (_SELECTION_BUTTON_PIN_NUMBER << 8) | (1 << 16);//event mode, hi to low
  NRF_GPIOTE->INTENSET = (1 << GPIOTE_CHANNEL_SELECT);

  NRF_GPIOTE->EVENTS_IN[GPIOTE_CHANNEL_CONFIRM] = 0;//clear the event flag
  NRF_GPIOTE->EVENTS_IN[GPIOTE_CHANNEL_SELECT] = 0;//clear the event flag
  NVIC_ClearPendingIRQ(GPIOTE_IRQn);
  NVIC_SetPriority(GPIOTE_IRQn, 3);
  NVIC_EnableIRQ(GPIOTE_IRQn);
  gpiote_set_button_select_func(&button_select_func);
  gpiote_set_button_confirm_func(&button_confirm_func);
}





static void button_select_func(void)
{
  switch (room_size_to_display)
  {
    case SMALL:
      room_size_to_display = MEDIUM;
      break;
    case MEDIUM:
      room_size_to_display = LARGE;
      break;
    case LARGE:
      room_size_to_display = SMALL;
      break;
  }
}

static void button_confirm_func(void)
{
  if (user_should_be_selecting)
  {
    user_has_selected_a_size = true;
    user_should_be_selecting = false;
  }
}
