/**
 * Where is the LED connected?
 * - Port?
 * - Pin?
 *
 * Pins on the microcontroller are grouped into ports. "PA1" means port A, pin 1, "PB2" means port B, pin 2, etc. The nucleo board
 * connects pins on the microcontroller to LEDs on the board.
 *
 * First we look at the user manual of the nucleo board, because the LED is part of the board, not the microcontroller. The user
 * manual is likely to tell us which port/pin of the microcontroller the board's LEDs are connected to.
 *
 * ```nucleo_user_guide.pdf
 * User LD1: a green user LED is connected to the STM32 I/O PB0 (SB120 ON and SB119
 * OFF) or PA5 (SB119 ON and SB120 OFF) corresponding to the ST Zio D13.
 * ```
 *
 * The above means that the green LED is connected to port B, pin 0 (PB0) when solder bridge 120 (SB120) is ON
 * and solder bridge 119 (SB119) is OFF, or to port A, pin 5 (PA5) when solder bridge 119 (SB119) is ON and
 * solder bridge 120 (SB120) is OFF.
 *
 * We can find on the back of our board that SB120 is connected (via a "zero Ohm" resistor) whereas
 * SB119 is not. Therefore we know that the green LED is connected to PB0.
 *
 * ```nucleo_user_guide.pdf
 * User LD2: a blue user LED is connected to PB7.
 * User LD3: a red user LED is connected to PB14.
 * ```
 *
 * GREEN LED = PB0 = Port B, Pin 0
 * BLUE LED = PB7 = Port B, Pin 7
 * RED LED = PB14 = Port B, Pin 14
 *
 * We now know that all of the LEDs in question are connected to port B, so the next step is to identify the address of port B.
 * Then there will be particular regions of that address space that correspond to the particular pins on port B. This general
 * concept is known as a "memory map".
 *
 * We can try searching for "memory map" in our datasheet, however in this case it just tells us to look in the reference manual.
 *
 * Eventually we can find `| 0x4002 0400 - 0x4002 07FF | GPIOB |` in the reference manual, which tells us the address range of the
 * GPIO peripheral defined as port B. We can also see that it is part of the larger peripheral bus AHB1, which starts at address
 * `0x4002 0000`, and that is part of a larger construct called the 512-Mbyte Block 2 Peripherals, which start at address `0x4000 0000`.
 */

/**
 * Each of the GPIO ports has four 32-bit memory-mapped control registers (GPIOx_MODER, GPIOx_OTYPER, GPIOx_OSPEEDR, GPIOx_PUPDR) to configure up to 16 I/Os.
 * The GPIOx_MODER register is used to select the I/O mode (input, output, AF, analog). The GPIOx_OTYPER and GPIOx_OSPEEDR registers are used to select the
 * output type (push- pull or open-drain) and speed. The GPIOx_PUPDR register is used to select the pull- up/pull-down whatever the I/O direction.
 *
 * Each GPIO has two 16-bit memory-mapped data registers: input and output data registers (GPIOx_IDR and GPIOx_ODR). GPIOx_ODR stores the data to be output,
 * it is read/write accessible. The data input through the I/O are stored into the input data register (GPIOx_IDR), a read-only register.
 *
 * The bit set reset register (GPIOx_BSRR) is a 32-bit register which allows the application to set and reset each individual bit in the output data register
 * (GPIOx_ODR). The bit set reset register has twice the size of GPIOx_ODR. To each bit in GPIOx_ODR, correspond two control bits in GPIOx_BSRR: BS(i) and BR(i).
 * When written to 1, bit BS(i) sets the corresponding ODR(i) bit. When written to 1, bit BR(i) resets the ODR(i) corresponding bit. Writing any bit to 0 in GPIOx_BSRR
 * does not have any effect on the corresponding bit in GPIOx_ODR. If there is an attempt to both set and reset a bit in GPIOx_BSRR, the set action takes priority.
 * Using the GPIOx_BSRR register to change the values of individual bits in GPIOx_ODR is a “one-shot” effect that does not lock the GPIOx_ODR bits. The GPIOx_ODR bits
 * can always be accessed directly. The GPIOx_BSRR register provides a way of performing atomic bitwise handling. There is no need for the software to disable interrupts
 * when programming the GPIOx_ODR at bit level: it is possible to modify one or more bits in a single atomic AHB write access.
 */

/**
 * In contemporary microcontrollers, clock access is often gated for each port, with the clock being turned off by default. This is done to save power, and is known as
 * "clock gating". In order to control this in software, we need to go through the RCC (Reset and Clock Control) module and figure out how to turn the clock gate on for
 * the GPIOB port.
 *
 * As we know, GPIOB is part of the AHB1 bus, so we will look for the clock gate controlling the AHB1 bus in the RCC registers.
 */

#define PERIPH_BASE 0x40000000UL
#define AHB1_OFFSET 0x00020000UL
#define GPIOB_OFFSET 0x0400UL
#define RCC_OFFSET 0x3800UL
#define AHB1PERIPH_BASE (PERIPH_BASE + AHB1_OFFSET)

// 5.3.10 RCC AHB1 peripheral clock register (RCC_AHB1ENR)
#define RCC_BASE (AHB1PERIPH_BASE + RCC_OFFSET)
#define RCC_AHB1ENR_OFFSET 0x30UL
#define RCC_AHB1ENR (*(volatile unsigned int *)(RCC_BASE + RCC_AHB1ENR_OFFSET))
// The reference manual tells us the 1st bit of RCC_AHB1ENR is what enables the GPIOB port.
#define RCC_AHB1ENR_GPIOBEN (1UL << 1)

#define GPIOB_BASE (AHB1PERIPH_BASE + GPIOB_OFFSET)
#define GPIOx_MODER_OFFSET 0x00UL

// 0x0000 0280 reset value
// 0b0000_0000_0000_0000_0000_0010_1000_0000
// See `GPIO port mode register (GPIOx_MODER) (x =A..K)` in the reference manual for the bit layout.
// Bits 2y+1:2y MODERy[1:0]: Portx configuration bits (y=0..15)
// We need pins 0, 7, and 14 to be outputs and the rest we don't care about.
// `01` is General purpose output mode
// Therefore, we need bits 0, 14, and 28 to be set to `1`.
#define GPIOB_MODER (*(volatile unsigned int *)(GPIOB_BASE + GPIOx_MODER_OFFSET))
#define GPIOB_MODER_MODER0_OUTPUT (1UL << 0)
#define GPIOB_MODER_MODER7_OUTPUT (1UL << 14)
#define GPIOB_MODER_MODER14_OUTPUT (1UL << 28)
#define LEDS_MODER (GPIOB_MODER_MODER0_OUTPUT | GPIOB_MODER_MODER7_OUTPUT | GPIOB_MODER_MODER14_OUTPUT)

#define GPIOx_ODR_OFFSET 0x14UL
#define GPIOB_ODR (*(volatile unsigned int *)(GPIOB_BASE + GPIOx_ODR_OFFSET))
#define GPIOB_ODR_ODR0 (1UL << 0)
#define GPIOB_ODR_ODR7 (1UL << 7)
#define GPIOB_ODR_ODR14 (1UL << 14)
#define USER_LED_1 GPIOB_ODR_ODR0  // Green LED
#define USER_LED_2 GPIOB_ODR_ODR7  // Blue LED
#define USER_LED_3 GPIOB_ODR_ODR14 // Red LED

// #define GPIOx_OTYPER_OFFSET 0x04UL
// #define GPIOx_OSPEEDR_OFFSET 0x08UL
// #define GPIOx_PUPDR_OFFSET 0x0CUL
// #define GPIOx_IDR_OFFSET 0x10UL
// #define GPIOx_BSRR_OFFSET 0x18UL
// #define GPIOx_LCKR_OFFSET 0x1CUL
// #define GPIOx_AFRL_OFFSET 0x20UL
// #define GPIOx_AFRH_OFFSET 0x24UL
// #define GPIOB_OTYPER (*(volatile unsigned int *))(GPIOB_BASE + GPIOx_OTYPER_OFFSET)
// #define GPIOB_OSPEEDR (*(volatile unsigned int *))(GPIOB_BASE + GPIOx_OSPEEDR_OFFSET)
// #define GPIOB_PUPDR (*(volatile unsigned int *))(GPIOB_BASE + GPIOx_PUPDR_OFFSET)
// #define GPIOB_IDR (*(volatile unsigned int *))(GPIOB_BASE + GPIOx_IDR_OFFSET)
// #define GPIOB_BSRR (*(volatile unsigned int *))(GPIOB_BASE + GPIOx_BSRR_OFFSET)
// #define GPIOB_LCKR (*(volatile unsigned int *))(GPIOB_BASE + GPIOx_LCKR_OFFSET)
// #define GPIOB_AFRL (GPIOB_BASE + GPIOx_AFRL_OFFSET)
// #define GPIOB_AFRH (GPIOB_BASE + GPIOx_AFRH_OFFSET)

int main()
{
  /* Enable clock access to port B */
  RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

  /* Set PB0, PB7, and PB14 to output mode */
  GPIOB_MODER |= LEDS_MODER;

  while (1)
  {
    /* Turn on all of our LEDs */
    // GPIOB_ODR |= USER_LED_1 | USER_LED_2 | USER_LED_3;

    /* Toggle all of our LEDs */
    GPIOB_ODR ^= USER_LED_1 | USER_LED_2 | USER_LED_3;
    for (int i = 0; i < 1000000; i++)
    {
    };
  }
}