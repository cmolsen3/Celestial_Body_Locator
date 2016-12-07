/*
 * uart.h
 *
 *  Created on: Nov 11, 2016
 *      Author: Clint Olsen
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>


void configure_serial_port0(void);
void configure_serial_port2(void);
void uart_putchar_n(uint8_t * data, uint32_t length);
void uart_putchar(uint8_t tx_data);


#endif /* UART_H_ */
