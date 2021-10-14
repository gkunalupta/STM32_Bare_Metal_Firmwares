/*
 * i2c.h
 *
 *  Created on: May 31, 2021
 *      Author: kunal
 */

#ifndef GB_I2C_H_
#define GB_I2C_H_

#include "stm32f1xx.h"
// I2C2 pin configuration, clock settings, interrupts, etc.
void gb_i2c2_master_init();

void gb_i2c2_start_condition_w();

void gb_i2c2_start_condition_r();

void gb_i2c2_address_send_w(uint8_t gb_slave_address);

int gb_i2c2_address_send_r(uint8_t gb_slave_address);
void gb_i2c2_master_send_byte(uint8_t gb_data);

void gb_i2c2_master_receive_2_bytes(uint8_t *gb_data1, uint8_t *gb_data2);

uint8_t gb_i2c2_master_receive_byte();

void gb_i2c2_master_stop_generation();

// I2C1 pin configuration, clock settings, interrupts, etc.
void gb_i2c1_master_init();

void gb_i2c1_start_condition_w();

void gb_i2c1_start_condition_r();

void gb_i2c1_address_send_w(uint8_t gb_slave_address);

int gb_i2c1_address_send_r(uint8_t gb_slave_address);
void gb_i2c1_master_send_byte(uint8_t gb_data);

void gb_i2c1_master_receive_2_bytes(uint8_t *gb_data1, uint8_t *gb_data2);

uint8_t gb_i2c1_master_receive_byte();

void gb_i2c1_master_stop_generation();


#endif /* GB_I2C_H_ */
