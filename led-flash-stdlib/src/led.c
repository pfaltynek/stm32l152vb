#include "led.h"
#include "stm32l1xx.h"
#include "stm32l152_eval.h"

const uint16_t leds[] = {
	GPIO_Pin_10,
	GPIO_Pin_11
};

void init_led(LEDS_enum led) {
	GPIO_InitTypeDef GPIO_InitStructure;

	// enable clock to GPIOE
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

	// Configure PD100 and PD111 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = leds[led];
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void set_led_on(LEDS_enum led) {
	GPIOE->BSRRH = leds[led];
}

void set_led_off(LEDS_enum led) {
	GPIOE->BSRRL = leds[led];
}

void toggle_led(LEDS_enum led) {
	GPIOE->ODR ^= leds[led];
}
