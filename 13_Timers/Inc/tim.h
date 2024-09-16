/*
 * tim.h
 *
 *  Created on: Sep 15, 2024
 *      Author: ibeckermayer
 */

#ifndef TIM_H_
#define TIM_H_

#include "stm32f7xx.h"

#define SR_UIF (1U<<0)

void tim1_1hz_init();

#endif /* TIM_H_ */
