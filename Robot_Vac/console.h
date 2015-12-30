#ifndef CONSOLE_H
#define CONSOLE_H

/*
Defines
*/

#define INPUT_LENGTH 20

/*
Typedefs
*/

typedef void(*func_ptr)(void);
typedef struct
{
  const char *name;
  func_ptr execute;
  const char *help;
} command;

/*
API functions
*/

const char * console_get_help_str(void);
bool console_go(void);

#endif
