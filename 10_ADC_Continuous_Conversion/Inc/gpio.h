/*
 * gpio.h
 *
 *  Created on: Aug 12, 2024
 *      Author: ibeckermayer
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f7xx.h"
#include <stdint.h>

void set_pin_mode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode);

#endif /* GPIO_H_ */
