#include "stm32f7xx.h"

#define GPIOB_CLK_EN (1U << 1) // 0000 0000 0000 0000 0000 0000 0000 0010
#define GPIOC_CLK_EN (1U << 2)

#define USER_LED1_MODER (1U << 0) // 0000 0000 0000 0000 0000 0000 0000 0001
#define USER_LED2_MODER (1U << 14)
#define USER_LED3_MODER (1U << 28)

#define USER_LED1 (1U << 0)  // Green Led
#define USER_LED2 (1U << 7)  // Blue Led
#define USER_LED3 (1U << 14) // Red Led

// B1 USER: the user button is connected to the I/O PC13 by default
// Ergo we need to enable the clock on Port C, set pin 13 on it to be an
// input pin.
#define PIN13 (1U<<13)
#define BTN_PIN PIN13

int main(void)
{
  /*Enable clock access to Port B*/
  RCC->AHB1ENR |= GPIOB_CLK_EN;
  RCC->AHB1ENR |= GPIOC_CLK_EN; // And port C

  /*Configure led pins as output pins*/
  GPIOB->MODER |= USER_LED1_MODER | USER_LED2_MODER | USER_LED3_MODER;

  /* Configure port 13 to be input gpio */
  GPIOC->MODER &=~(1U<<26);
  GPIOC->MODER &=~(1U<<27);

  while (1)
  {
	  /* Check if button is pressed */
	  if (GPIOC->IDR & BTN_PIN) {
		  // If button is pressed, turn on all the LEDs
		  GPIOB->BSRR = (1U << 0) | (1U << 7) | (1U << 14);
	  } else {
		  // Otherwise, turn them all off
		  GPIOB->BSRR = (1U << 16) | (1U << 23) | (1U << 30);
	  }
  }
}
