#include <stdint.h>

#define PERIPH_BASE 0x40000000UL
#define AHB1_OFFSET 0x00020000UL
#define AHB1PERIPH_BASE (PERIPH_BASE + AHB1_OFFSET)

#define RCC_OFFSET 0x3800UL
#define RCC_BASE (AHB1PERIPH_BASE + RCC_OFFSET)
#define RCC_AHB1ENR_GPIOBEN (1UL << 1)

#define GPIOB_OFFSET 0x0400UL
#define GPIOB_BASE (AHB1PERIPH_BASE + GPIOB_OFFSET)

#define GPIOB_MODER_MODER0_OUTPUT (1UL << 0)
#define GPIOB_MODER_MODER7_OUTPUT (1UL << 14)
#define GPIOB_MODER_MODER14_OUTPUT (1UL << 28)
#define LEDS_MODER (GPIOB_MODER_MODER0_OUTPUT | GPIOB_MODER_MODER7_OUTPUT | GPIOB_MODER_MODER14_OUTPUT)

#define GPIOx_ODR_OFFSET 0x14UL
#define GPIOB_ODR_ODR0 (1UL << 0)
#define GPIOB_ODR_ODR7 (1UL << 7)
#define GPIOB_ODR_ODR14 (1UL << 14)
#define USER_LED_1 GPIOB_ODR_ODR0  // Green LED
#define USER_LED_2 GPIOB_ODR_ODR7  // Blue LED
#define USER_LED_3 GPIOB_ODR_ODR14 // Red LED

typedef struct
{
  volatile uint32_t DUMMY[12]; // 48 byte offset to the AHB1ENR register
  volatile uint32_t AHB1ENR;
} RCC_TypeDef;

typedef struct
{
  volatile uint32_t MODER;
  volatile uint32_t DUMMY[4]; // 16 byte offset to the ODR register
  volatile uint32_t ODR;
} GPIO_TypeDef;

#define RCC ((RCC_TypeDef *)RCC_BASE)
#define GPIOB ((GPIO_TypeDef *)GPIOB_BASE)

int main()
{
  /* Enable clock access to port B */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

  /* Set PB0, PB7, and PB14 to output mode */
  GPIOB->MODER |= LEDS_MODER;

  while (1)
  {
    /* Turn on all of our LEDs */
    // GPIOB_ODR |= USER_LED_1 | USER_LED_2 | USER_LED_3;

    /* Toggle all of our LEDs */
    GPIOB->ODR ^= USER_LED_1 | USER_LED_2 | USER_LED_3;
    for (int i = 0; i < 1000000; i++)
    {
    };
  }
}
