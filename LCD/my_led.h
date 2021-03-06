#ifndef MY_LED_H
#define MY_LED_H

#define LED1 (1 << 21)
#define LED2 (1 << 22)
#define LED3 (1 << 23)
#define LED4 (1 << 24)

void led_all_off(void);
void led_blink_all(void);
void led_init(void);
void led_pattern_dance(void);
void led_toggle(uint32_t led);

#endif /*MY_LED_H*/
