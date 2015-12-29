#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "my_strings.h"


bool strings_compare(const char *str1, const char *str2)
{
  while ((*str1 != '\0') && (*str2 != '\0'))
  {
    if (*str1++ != *str2++)
      return false;
  }

  return ((*str1 == '\0') && (*str2 == '\0'));
}
