#ifndef CONSOLE_H
#define CONSOLE_H

/*
Defines
*/

#define INPUT_LENGTH 20

/*
Typedefs
*/

typedef void(*function_pointer)(void);
typedef struct
{
  const char *name;
  function_pointer execute;
  const char *help;
} command;

/*
API functions
*/

const char * console_get_help_str(void);
bool console_go(void);

#endif
