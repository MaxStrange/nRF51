#ifndef MY_LED_H
#define MY_LED_H

#define _LED1_PIN_NUMBER 21
#define _LED2_PIN_NUMBER 22
#define _LED3_PIN_NUMBER 23
#define _LED4_PIN_NUMBER 24

#define LED1 (1 << _LED1_PIN_NUMBER)
#define LED2 (1 << _LED2_PIN_NUMBER)
#define LED3 (1 << _LED3_PIN_NUMBER)
#define LED4 (1 << _LED4_PIN_NUMBER)

void led_all_off(void);
void led_all_on(void);
void led_blink_all(void);
void led_dim_high(uint32_t duration_ms);
void led_dim_low(uint32_t duration_ms);
void led_init(void);
void led_pattern_breathe(void);
void led_pattern_dance(void);
void led_toggle(uint32_t led);
void led_turn_off(uint32_t led);
void led_turn_on(uint32_t led);

#endif /*MY_LED_H*/
