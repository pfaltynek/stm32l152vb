#ifndef LED_H
#define LED_H

typedef enum {
	LED_GREEN = 0,
	LED_ORANGE = 1
} LEDS_enum;

void init_led(LEDS_enum led);
void set_led_on(LEDS_enum led);
void set_led_off(LEDS_enum led);
void toggle_led(LEDS_enum led);
#endif
