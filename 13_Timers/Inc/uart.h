/*
 * uart.h
 *
 *  Created on: Aug 12, 2024
 *      Author: ibeckermayer
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f7xx.h"
#include <stdint.h>

void uart3_rx_tx_init();
uint8_t uart_read(USART_TypeDef *USARTx);

#endif /* UART_H_ */
