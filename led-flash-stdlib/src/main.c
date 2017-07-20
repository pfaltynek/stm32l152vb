#include "button.h"
#include "led.h"
#include "stm32l1xx.h" // due temporary use of __IO

void Delay(__IO uint32_t nCount) {
	while (nCount--) {
	}
}

uint32_t led_speed[] = {3000, 2000, 1500, 1000, 500, 5000};
uint8_t led_speed_at = 0;
uint8_t led_speed_count = 6;

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

void enable_interrupt(IRQn_Type interrupt,
					  uint8_t preemption_prio,
					  uint8_t sub_prio) {
	__disable_irq();

	NVIC_InitTypeDef interr;
	interr.NVIC_IRQChannel = interrupt;
	interr.NVIC_IRQChannelPreemptionPriority = preemption_prio;
	interr.NVIC_IRQChannelSubPriority = sub_prio;
	interr.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&interr);

	__enable_irq();
}

void tim2_interrupt_handler() {
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
	toggle_led(LED_ORANGE);
}

void exti15_10_interrupt_handler() {
	if (EXTI_GetFlagStatus(EXTI_Line13) == SET) {
		toggle_led(LED_GREEN);

		TIM_SetAutoreload(TIM2, led_speed[led_speed_at]);
		TIM_SetCounter(TIM2, 0);
		led_speed_at++;
		if (led_speed_at == led_speed_count) {
			led_speed_at = 0;
		}

		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}

int main(void) {
    SystemInit();

#ifndef  EMB_FLASH
    /* Set the Vector Table base location at 0x20000000 */
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	init_led(LED_ORANGE);
	init_led(LED_GREEN);

	init_button();

	enable_button();
	enable_timer(TIM2, RCC_APB1Periph_TIM2, 3200, 5000);

	set_led_on(LED_ORANGE);
	set_led_off(LED_GREEN);

    enable_interrupt(TIM2_IRQn, 7, 0);
    enable_interrupt(EXTI15_10_IRQn, 7, 1);

    while (1) {}

	while (1) {
		// delay
		Delay(5000000);

		// toggle leds
		toggle_led(LED_ORANGE);
		toggle_led(LED_GREEN);

	}
}
