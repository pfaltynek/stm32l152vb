#include "stm32l1xx.h"
#include "stm32l152_eval.h"

#define GPIO_PIN_X GPIO_Pin_10
#define GPIO_PIN_Y GPIO_Pin_11
#define BSRR_VAL0 0x0C00
#define BSRR_VAL1 0x0800
#define BSRR_VAL2 0x0400

GPIO_InitTypeDef GPIO_InitStructure;

void Delay(__IO uint32_t nCount) {
	while (nCount--) {
	}
}
int main(void) {
	// GPIOE Periph clock enable
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

	// Configure PD100 and PD111 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_X | GPIO_PIN_Y;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	// Set LEDs off
	GPIOE->BSRRL = BSRR_VAL0;

	while (1) {
		// Set GREEN LED on and ORANGE LED off
		GPIOE->BSRRL = BSRR_VAL1;
		GPIOE->BSRRH = BSRR_VAL2;

		Delay(5000000);

		// Set GREEN LED off and ORANGE LED on
		GPIOE->BSRRH = BSRR_VAL1;
		GPIOE->BSRRL = BSRR_VAL2;

		Delay(10000000);
	}
}
