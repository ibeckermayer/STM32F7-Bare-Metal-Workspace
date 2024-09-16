#include "stm32f7xx.h"
#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "led.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

int sensor_value;

int main(void)
{
	uart3_rx_tx_init();
	user_leds_init();
	tim1_1hz_init();

	while (1)
	{
		/**
		 * Wait for UIF to be set, which means the
		 * counter we set up in tim1_1hz_init has
		 * overflowed.
		 */
		while(!(TIM1->SR & SR_UIF)) {}
		/**
		 * Clear the UIF bit.
		 */
		TIM1->SR &= ~SR_UIF;
		printf("A second just passed!\r\n");
		led_toggle(USER_LED1);
	}
}
