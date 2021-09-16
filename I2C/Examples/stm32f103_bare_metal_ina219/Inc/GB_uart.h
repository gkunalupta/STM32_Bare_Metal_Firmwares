/*
 * uart.h
 *
 *  Created on: 21-Feb-2021
 *      Author: kunal
 */

#ifndef __GB_UART_H_
#define __GB_UART_H_
#include "stm32f1xx.h"
#include <string.h>

/********************STM32 -UART1 ********************/
void GB_uart_pin_conf1();
void GB_uart_boud_rate1();
void GB_uart_init1();
void GB_UART_TxChar1(uint32_t byte);
uint16_t GB_UART_RxChar1();
void GB_printString1(const char *myString);      /////to print any string
void GB_bit1(uint32_t val);
void GB_decimel1(uint32_t val);
void GB_uart_newline1();
void GB_uart_rec1(char *buff);
void GB_float_value1(float gb_value);

//************************STM32- UART2 functions*******************//
//PA3- RX, PA2-TX
void GB_uart_pin_conf2();
void GB_uart_boud_rate2();
void GB_uart_init2();
void GB_UART_TxChar2(uint32_t byte);
uint16_t GB_UART_RxChar2();
void GB_printString2(const char *myString);      /////to print any string
void GB_bit2(uint32_t val);
void GB_decimel2(uint32_t val);
void GB_uart_newline2();
void GB_uart_rec2(char *buff);
void GB_float_value2(float gb_value);


#endif /* UART_H_ */
