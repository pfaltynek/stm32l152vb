#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "stm32l1xx.h"

void enable_interrupt(IRQn_Type interrupt, uint8_t preemption_prio, uint8_t sub_prio);
void tim2_interrupt_handler(void);
void exti15_10_interrupt_handler(void);
#endif 