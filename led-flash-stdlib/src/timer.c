#include "timer.h"

uint32_t led_blink_speeds[] = {5000, 3000, 2000, 1500, 1000, 500};
uint8_t led_speed_at = 0;
uint8_t led_speed_count = 6;

uint32_t get_next_led_blink_speed(void) {
	uint32_t result = led_blink_speeds[led_speed_at];
	led_speed_at++;
	led_speed_at = led_speed_at % led_speed_count;

	return result;
}

void enable_timer(TIM_TypeDef *timer,
				  uint32_t enable_reg,
				  uint16_t prescaler,
				  uint32_t period) {
	TIM_TimeBaseInitTypeDef timer_init_struct;

	// Enable peripheral clock for timer.
	RCC_APB1PeriphClockCmd(enable_reg, ENABLE);
	RCC_APB1PeriphResetCmd(enable_reg, DISABLE);

	timer_init_struct.TIM_Prescaler = prescaler;
	// Here we set counter mode to up.
	timer_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
	timer_init_struct.TIM_Period = period;
	timer_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(timer, &timer_init_struct);

	TIM_ClearITPendingBit(timer, TIM_FLAG_Update);
	TIM_ITConfig(timer, TIM_FLAG_Update, ENABLE);

	TIM_Cmd(timer, ENABLE);
}
