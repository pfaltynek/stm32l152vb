#include "button.h"
#include "led.h"
#include "interrupt.h"
#include "timer.h"
#include "stm32l1xx.h"

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
	enable_timer(TIM2, RCC_APB1Periph_TIM2, 3200, get_next_led_blink_speed());

	set_led_on(LED_ORANGE);
	set_led_off(LED_GREEN);

    enable_interrupt(TIM2_IRQn, 7, 0);
    enable_interrupt(EXTI15_10_IRQn, 7, 1);

    while (1) {}
}
