/*
 * uart.c
 *
 *  Created on: Aug 12, 2024
 *      Author: ibeckermayer
 */

#include "uart.h"
#include "rcc.h"
#include "gpio.h"

#define GPIODEN (1U << 3)
#define UART_TX_PIN 8
#define GPIO_ALTERNATE_MODE 0x2 // 0b10, from reference manual
#define USART3EN (1U << 18)
#define UART_DATAWIDTH_8B 0x00000000U
#define UART_PARITY_NONE 0x00000000U
#define UART_STOPBIT_1 0x00000000U

static uint16_t compute_uart_div(uint32_t PeriphClk, uint32_t Baudrate);
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t Baudrate);
static void uart_enable();
static void uart_write(USART_TypeDef *USARTx, uint8_t value);
static void set_uart_transfer_direction(USART_TypeDef *USARTx, uint32_t TransferDirection);
static void uart_parameters_config(USART_TypeDef *USARTx, uint32_t Datawidth, uint32_t Parity, uint32_t StopBits);

int __io_putchar(int ch) {
	uart_write(USART3, ch);
	return ch;
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

static void set_uart_transfer_direction(USART_TypeDef *USARTx, uint32_t TransferDirection)
{
  MODIFY_REG(USARTx->CR1, USART_CR1_RE | USART_CR1_TE, TransferDirection);
}

static void uart_enable()
{
  // USART3->CR1 |= USART_CR1_UE;
  SET_BIT(USART3->CR1, USART_CR1_UE);
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t Baudrate)
{
  USARTx->BRR = compute_uart_div(PeriphClk, Baudrate);
}

static void uart_parameters_config(USART_TypeDef *USARTx, uint32_t Datawidth, uint32_t Parity, uint32_t StopBits)
{
  MODIFY_REG(USARTx->CR1, USART_CR1_PS | USART_CR1_PCE | USART_CR1_M, Parity | Datawidth);
  MODIFY_REG(USARTx->CR2, USART_CR2_STOP, StopBits);
}

static uint16_t compute_uart_div(uint32_t PeriphClk, uint32_t Baudrate)
{
  return ((PeriphClk + (Baudrate / 2U)) / Baudrate);
}
