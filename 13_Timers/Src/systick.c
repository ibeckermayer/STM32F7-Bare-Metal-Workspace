/*
 * systick.c
 *
 *  Created on: Sep 15, 2024
 *      Author: ibeckermayer
 */


#include "systick.h"

/*
 * Number of clock cycles per millisecond given a 16MHz clock
 */
#define SYSTICK_LOAD_VAL 16000
#define CTRL_ENABLE (1U<<0)
#define CTRL_CLKSRC (1U<<2)
#define CTRL_COUNTFLAG (1U<<16)

void systickDelayMs(int delay) {
	/* AKA RELOAD register; we give it the number of clock cycles per millisecond */
	SysTick->LOAD = SYSTICK_LOAD_VAL;
	/* Clear the systick current value register */
	SysTick->VAL = 0;
	/* Select clock source and enable systick */
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	for (int i = 0; i < delay; i++) {
		/* Wait until COUNTFLAG is set */
		while ((SysTick->CTRL & CTRL_COUNTFLAG) == 0) {}
	}

	// A write of any value clears the field to 0, and also clears the CTRL_COUNTFLAG bit to 0.
	SysTick->VAL = 0;
}
