/*
 * led.c
 *
 *  Created on: Aug 12, 2024
 *      Author: ibeckermayer
 */

#include "led.h"

void user_leds_init()
{
    RCC->AHB1ENR |= GPIOB_CLK_EN;

    // Set PB0 as output
    GPIOB->MODER &= ~(3U << 0);  // Clear MODER0[1:0] bits
    GPIOB->MODER |= (1U << 0);   // Set MODER0[1:0] to 01 (output mode)

    // Set PB7 as output
    GPIOB->MODER &= ~(3U << 14); // Clear MODER7[1:0] bits
    GPIOB->MODER |= (1U << 14);  // Set MODER7[1:0] to 01 (output mode)

    // Set PB14 as output
    GPIOB->MODER &= ~(3U << 28); // Clear MODER14[1:0] bits
    GPIOB->MODER |= (1U << 28);  // Set MODER14[1:0] to 01 (output mode)
}

void all_leds_on()
{
	GPIOB->ODR |= (RED_LED | GREEN_LED | BLUE_LED);
}

void all_leds_off()
{
	GPIOB->ODR &= ~(RED_LED | GREEN_LED | BLUE_LED);
}

void all_leds_toggle()
{
	GPIOB->ODR ^= (RED_LED | GREEN_LED | BLUE_LED);
}

void led_toggle(ledType led)
{
	GPIOB->ODR ^= led;
}

void led_on(ledType led)
{
	GPIOB->ODR |= led;
}

void led_off(ledType led)
{
	GPIOB->ODR &= ~led;
}
