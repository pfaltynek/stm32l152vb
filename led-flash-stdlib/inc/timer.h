#ifndef TIMER_H
#define TIMER_H
#include "stm32l1xx.h"

uint32_t get_next_led_blink_speed(void);
void enable_timer(TIM_TypeDef *timer, uint32_t enable_reg, uint16_t prescaler, uint32_t period);
#endif