#include "interrupt.h"
#include "led.h"
#include "timer.h"

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

		TIM_SetAutoreload(TIM2, get_next_led_blink_speed());
		TIM_SetCounter(TIM2, 0);

		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}