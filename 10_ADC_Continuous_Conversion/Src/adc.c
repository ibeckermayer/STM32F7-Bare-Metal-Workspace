/*
 * adc.c
 *
 *  Created on: Sep 8, 2024
 *      Author: ibeckermayer
 */
#include "adc.h"
#define GPIOAEN (1U<<0)
#define ADC1EN (1U<<8)
#define CR2_CONT (1U<<1)
#define CR2_ADON (1U<<0)
#define CR2_SWSTART (1U<<30)
#define CH4_RANK1 (1U<<2) // 0b0100
#define SR_EOC (1U<<1)


void pa4_adc1_init() {
	// Configure the ADC gpio pin
	// Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;
	// Set PA4 to analog mode
	GPIOA->MODER |= (1U<<8);
	GPIOA->MODER |= (1U<<9);

	// Configure the ADC module (APB2 bus)
	// Enable clock access to the ADC module
	RCC->APB2ENR |= ADC1EN;
	// Set conversion to continuous
	ADC1->CR2 |= CR2_CONT;

	// Set the sequencer length to 1:
	// ADC1->SQR1 bits 20-23 must be zero. This is the reset value, so do nothing

	// Set ADC channel
	// PA4 is "ADC1_IN4", so we have to set the SQ1 value to "4"
	// This says "sample input 4 first". Since we're only sampling one pin
	// this is a bit tedious here, but if there were others then it would be
	// relevant to set e.g. SQ1 = 4, SQ2 = 1, etc. so that analog sensors are
	// sampled in the order we want them to be by the given ADC module.
	ADC1->SQR3 |= CH4_RANK1;

	// Enable ADC module
	ADC1->CR2 |= CR2_ADON;
	// Start ADC
	// SWSTART: Start conversion of regular channels
	// This bit is set by software to start conversion and cleared by hardware as soon as the conversion starts.
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_get_data() {
	// Wait for End-of-Conversion flag
	while (!(ADC1->SR & SR_EOC)){}
	// Read results
	return (ADC1->DR);
}
