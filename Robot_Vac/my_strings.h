#ifndef MY_STRINGS_H
#define MY_STRINGS_H


bool strings_compare(const char *str1, const char *str2);
void strings_int_to_str(uint16_t i, volatile char * buffer);
void strings_reverse(volatile char * str);

#endif //MY_STRINGS_H
