/*
 * uart.h
 *
 *  Created on: 21-Feb-2021
 *      Author: kunal
 */

#ifndef __UART_H_
#define __UART_H_
#include "stm32f1xx.h"


//************************STM32- UART2 functions*******************//
//PA3- RX, PA2-TX
void uart2_pin_conf();
void uart_boud_rate();
void uart_init();
void uart_tran_byte(uint8_t byte);
uint16_t uart_rec_byte();
void uart_tran_string(const char *myString);      /////to print any string
void uart_tran_bin(uint32_t val);
void uart_tran_dec(uint32_t val);
void newlin_uart();
void uart_rec(char *buff);

//#include <uart.cpp>

#endif /* UART_H_ */
