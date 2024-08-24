#include "stm32f7xx.h"
#include <stdio.h>

#define GPIODEN (1U << 3)
#define UART_TX_PIN 8
#define GPIO_ALTERNATE_MODE 0x2 // 0b10, from reference manual
#define USART3EN (1U << 18)
#define UART_DATAWIDTH_8B 0x00000000U
#define UART_PARITY_NONE 0x00000000U
#define UART_STOPBIT_1 0x00000000U

void set_ahb1_periph_clock(uint32_t periphs);
void set_pin_mode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode);
void set_apb1_periph_clock(uint32_t periphs);
void uart_parameters_config(USART_TypeDef *USARTx, uint32_t Datawidth, uint32_t Parity, uint32_t StopBits);
uint16_t compute_uart_div(uint32_t PeriphClk, uint32_t Baudrate);
void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t Baudrate);
void uart_enable();
void uart_write(USART_TypeDef *USARTx, uint8_t value);
void set_uart_transfer_direction(USART_TypeDef *USARTx, uint32_t TransferDirection);

int __io_putchar(int ch) {
	uart_write(USART3, ch);
	return ch;
}

int main(void)
{
  int x;
  uart3_rx_tx_init();
  while (1)
  {
    printf("Hello from STM32F7...\r\n");
    // uart_write(USART3, '\r');
    // uart_write(USART3, '\n');
    for (int i = 0; i < 9000; i++)
    {
      x++; // prevents compiler from optimizing this away
    }
  }
}

void set_uart_transfer_direction(USART_TypeDef *USARTx, uint32_t TransferDirection)
{
  MODIFY_REG(USARTx->CR1, USART_CR1_RE | USART_CR1_TE, TransferDirection);
}

void uart_write(USART_TypeDef *USARTx, uint8_t value)
{
  /*Make sure transmit data register is empty*/
  // while (!((USARTx->ISR & USART_ISR_TXE) == USART_ISR_TXE))
  while (!((USARTx->ISR & USART_ISR_TXE)))
  {
  }
  /*Write value into transmit data register*/
  USARTx->TDR = value;
}

void uart3_rx_tx_init()
{
  // 1. enable clock access to gpiod
  set_ahb1_periph_clock(GPIODEN);
  // 2. set pd8 to mode to alternate function
  set_pin_mode(GPIOD, UART_TX_PIN, GPIO_ALTERNATE_MODE);
  // 3. set alternate function type to usart
  // 0111 = AF7 = uart
  GPIOD->AFR[1] |= 7U;
  // 4. enable clock access to the UART module
  set_apb1_periph_clock(USART3EN);
  // 5. configure usart parameters
  uart_parameters_config(USART3, UART_DATAWIDTH_8B, UART_PARITY_NONE, UART_STOPBIT_1);
  set_uart_transfer_direction(USART3, USART_CR1_TE);
  // 6. set buadrate
  uart_set_baudrate(USART3, 16000000, 115200);
  // 7. enable usart
  uart_enable();
}

void uart_enable()
{
  // USART3->CR1 |= USART_CR1_UE;
  SET_BIT(USART3->CR1, USART_CR1_UE);
}

void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t Baudrate)
{
  USARTx->BRR = compute_uart_div(PeriphClk, Baudrate);
}

void uart_parameters_config(USART_TypeDef *USARTx, uint32_t Datawidth, uint32_t Parity, uint32_t StopBits)
{
  MODIFY_REG(USARTx->CR1, USART_CR1_PS | USART_CR1_PCE | USART_CR1_M, Parity | Datawidth);
  MODIFY_REG(USARTx->CR2, USART_CR2_STOP, StopBits);
}

void set_ahb1_periph_clock(uint32_t periphs)
{
  // RCC->AHB1ENR |= periphs;
  SET_BIT(RCC->AHB1ENR, periphs);
}

void set_ahb2_periph_clock(uint32_t periphs)
{
  SET_BIT(RCC->AHB2ENR, periphs);
}

void set_apb1_periph_clock(uint32_t periphs)
{
  SET_BIT(RCC->APB1ENR, periphs);
}

void set_apb2_periph_clock(uint32_t periphs)
{
  SET_BIT(RCC->APB2ENR, periphs);
}

void set_pin_mode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
{
  // Clear the relevant bits in the mode register
  // GPIOD->MODER &=~(1U<<16);
  // GPIOD->MODER &=~(1U<<17);
  // Set the relevant bits in the mode register
  // GPIOD->MODER |=(1U<<17);

  /**
   * CLEARMASK is  0b11 << 16 to get 1's on bits 16 and 17
   * In hex that's 0x03 << 16
   */

  MODIFY_REG(GPIOx->MODER, 0x03 << (Pin * 2), Mode << (Pin * 2));
}

uint16_t compute_uart_div(uint32_t PeriphClk, uint32_t Baudrate)
{
  return ((PeriphClk + (Baudrate / 2U)) / Baudrate);
}
