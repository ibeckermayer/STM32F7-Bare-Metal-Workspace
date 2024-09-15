/*
 * gpio.c
 *
 *  Created on: Aug 12, 2024
 *      Author: ibeckermayer
 */

#include "gpio.h"

void set_pin_mode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
{
  // Clear the relevant bits in the mode register
  // GPIOD->MODER &=~(1U<<16);
  // GPIOD->MODER &=~(1U<<17);
  // Set the relevant bits in the mode register
  // GPIOD->MODER |=(1U<<17);

  /**
   * CLEARMASK is  0b11 << 16 to get 1's on bits 16 and 17
   * In hex that's 0x03 << 16
   */

  MODIFY_REG(GPIOx->MODER, 0x03 << (Pin * 2), Mode << (Pin * 2));
}


