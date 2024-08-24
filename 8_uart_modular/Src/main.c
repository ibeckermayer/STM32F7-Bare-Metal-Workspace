#include "stm32f7xx.h"
#include <stdio.h>
#include "uart.h"
#include "gpio.h"

int main(void)
{
  int x;
  uart3_rx_tx_init();
  while (1)
  {
    printf("Hello from STM32F7... modular!!\r\n");
    // uart_write(USART3, '\r');
    // uart_write(USART3, '\n');
    for (int i = 0; i < 9000; i++)
    {
      x++; // prevents compiler from optimizing this away
    }
  }
}




