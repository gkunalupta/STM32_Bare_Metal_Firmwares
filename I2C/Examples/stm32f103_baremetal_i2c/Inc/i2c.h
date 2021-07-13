/*
 * i2c.h
 *
 *  Created on: May 31, 2021
 *      Author: kunal
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f1xx.h"

void master_init();

void i2c_start_condition_w();

void i2c_start_condition_r();

void i2c_address_send_w(uint8_t slave_address);

int i2c_address_send_r(uint8_t slave_address);
void databyte_send(uint8_t data);

void databyte_receive_ack(uint8_t data1, uint8_t data2);

uint8_t databyte_receive_nack();

void lastdata_send(uint8_t data);

void stop_generation();


#endif /* I2C_H_ */
