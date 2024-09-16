/*
 * tim.c
 *
 *  Created on: Sep 15, 2024
 *      Author: ibeckermayer
 */


#include "tim.h"

#define TIM1EN (1U<<0)
#define CR1_CEN (1U<<0)

void tim1_1hz_init() {
	/* Enable clock access to TIM1 */
	RCC->APB2ENR |= TIM1EN;
	/* Set the prescaler */
	TIM1->PSC = 1600 - 1; // 16_000_000 / 1_600 = 10_000 Hz
	/* Set the auto-reload value */
	TIM1->ARR = 10000 - 1; // Count up to 10000 for a 10KHz clock = 1Hz per timeout
	/* Enable the timer */
	TIM1->CR1 |= CR1_CEN;
}
