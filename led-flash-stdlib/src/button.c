#include "button.h"
#include "stm32l1xx.h"

void enable_button() {
	EXTI_InitTypeDef init;

	// The button is connected to line 13 on the exti.
	init.EXTI_Line = EXTI_Line13;
	// We want it to be an interrupt and not an event.
	init.EXTI_Mode = EXTI_Mode_Interrupt;
	// Trigger interrupt on rising edge.
	init.EXTI_Trigger = EXTI_Trigger_Rising;
	init.EXTI_LineCmd = ENABLE;

	EXTI_Init(&init);

	// We need to enable this clock for pin 13 on gpio port C to
	// get interrupt when the edge changes, otherwise you'll get interrupts
	// all the time.
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
}

void init_button() {
	GPIO_InitTypeDef init;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	init.GPIO_Pin = GPIO_Pin_13;
	init.GPIO_Mode = GPIO_Mode_IN;
	init.GPIO_PuPd = GPIO_PuPd_DOWN;

    GPIO_Init(GPIOC, &init);
}
