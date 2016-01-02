#ifndef _USER_INPUT_H_
#define _USER_INPUT_H_

#define _CONFIRM_BUTTON_PIN_NUMBER    14
#define _SELECTION_BUTTON_PIN_NUMBER  15

#define CONFIRM_BUTTON    (1 << _CONFIRM_BUTTON_PIN_NUMBER)
#define SELECTION_BUTTON  (1 << _SELECTION_BUTTON_PIN_NUMBER)


typedef enum
{
  SMALL,
  MEDIUM,
  LARGE
} e_room_size_t;

e_room_size_t user_input_get_room_size(void);
void user_input_init(void);

#endif //_USER_INPUT_H_
