#include "stm32f7xx.h"
#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "led.h"
#include "adc.h"
#include "systick.h"

int sensor_value;

int main(void)
{
	uart3_rx_tx_init();
	user_leds_init();

	while (1)
	{
		systickDelayMs(1000);
		printf("A second just passed!\r\n");
		all_leds_toggle();
	}
}
