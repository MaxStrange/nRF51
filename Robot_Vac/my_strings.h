#ifndef MY_STRINGS_H
#define MY_STRINGS_H

bool strings_compare(const char *str1, const char *str2);
void strings_concatenate(const char *left, const char *right, volatile char *buffer);
uint32_t strings_get_length(const char *str);
void strings_int_to_str(uint16_t i, volatile char * buffer);
void strings_reverse(volatile char * str);
void strings_split(uint16_t index, volatile const char *str, char *left_buffer, char *right_buffer);

#endif //MY_STRINGS_H
