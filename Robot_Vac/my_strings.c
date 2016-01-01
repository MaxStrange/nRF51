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

void strings_concatenate(const char *left, const char *right, volatile char *buffer)
{
  uint32_t i = 0;
  while (*left != '\0')
  {
    buffer[i] = *left++;
    i++;
  }

  while (*right != '\0')
  {
    buffer[i] = *right++;
    i++;
  }

  buffer[i] = '\0';
}

/*
Returns the length WITHOUT the nul terminator.
*/
uint32_t strings_get_length(const char *str)
{
  uint32_t len = 0;
  while (*str++ != '\0')
    len++;

  return len;
}

void strings_int_to_str(uint16_t i, volatile char * buffer)
{
  if (i == 0)
  {
    buffer[0] = '0';
    buffer[1] = '\0';
  }
  else
  {
    uint8_t index = 0;
    while (i != 0)
    {
      uint8_t j = (i % 10) + '0';
      buffer[index] = j;

      i /= 10;
      index++;
    }

    buffer[index] = '\0';
    strings_reverse(buffer);
  }
}

void strings_reverse(volatile char * str)
{
  volatile char *first = str;
  volatile char *last = str;
  uint16_t len = 0;
  while (*last != '\0')
  {
    last++;
    len++;
  }
  last--;

  char tmp;
  uint16_t i = 0;
  for (i = 0; i < len / 2; i++)
  {
    tmp = *last;
    *last = *first;
    *first = tmp;

    last--;
    first++;
  }
}

/*
Splits the given string at the given index. Left string will
end up with the indexed item as its last item before the nul.
*/
void strings_split(uint16_t index, volatile const char *str, char *left_buffer, char *right_buffer)
{
  uint16_t i = 0;
  while ((*str != '\0') && (i != index))
  {
    left_buffer[i] = *str++;
    i++;
  }

  left_buffer[i] = *str++;
  left_buffer[i + 1] = '\0';
  i = 0;

  while (*str != '\0')
  {
    right_buffer[i] = *str++;
    i++;
  }

  right_buffer[i] = '\0';
}
