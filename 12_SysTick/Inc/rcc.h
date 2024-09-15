/*
 * rcc.h
 *
 *  Created on: Aug 12, 2024
 *      Author: ibeckermayer
 */

#ifndef RCC_H_
#define RCC_H_

#include "stm32f7xx.h"

void set_ahb1_periph_clock(uint32_t periphs);
void set_apb1_periph_clock(uint32_t periphs);


#endif /* RCC_H_ */
