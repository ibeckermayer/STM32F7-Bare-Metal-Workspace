#include "stm32f7xx.h"
#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "led.h"

char ch;

int main(void)
{
  uart3_rx_tx_init();
  user_leds_init();
  printf("write is working\r\n");

  while (1)
  {
    ch = uart_read(USART3);
    switch (ch)
    {
    case '0':
      all_leds_off();
      break;
    case 'a':
      all_leds_on();
      break;
    case 'b':
      led_toggle(BLUE_LED);
      break;
    case 'g':
      led_toggle(GREEN_LED);
      break;
    case 'r':
      led_toggle(RED_LED);
      break;
    }
  }
}
