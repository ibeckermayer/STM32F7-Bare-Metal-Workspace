#include "stm32f7xx.h"
#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "led.h"
#include "adc.h"

int sensor_value;

int main(void)
{
  pa4_adc1_init();
  uart3_rx_tx_init();

  while (1)
  {
    sensor_value = adc_get_data();
    printf("sensor_value = %d\r\n", sensor_value);
  }
}
