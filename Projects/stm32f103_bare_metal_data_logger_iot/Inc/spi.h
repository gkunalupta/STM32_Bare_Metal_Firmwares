/*
 * spi.h
 *
 *  Created on: 27-Mar-2021
 *      Author: kunal
 */

#ifndef SPI_H_
#define SPI_H_
#include "stm32f1xx.h"

void spi_pin_conf();


void spi_init();

void spi_mast_tran_byte(uint8_t data);




uint8_t spi_mast_exchange_byte(uint8_t data);


uint8_t spi_mast_recv_byte();


#endif /* SPI_H_ */
