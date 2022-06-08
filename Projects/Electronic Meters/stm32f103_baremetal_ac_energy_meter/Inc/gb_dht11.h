/*
 * gb_dht11.h
 *
 *  Created on: Oct 28, 2021
 *      Author: kunal
 */

#ifndef GB_DHT11_H_
#define GB_DHT11_H_

#define RESPONSE_PULSE_MASK 0x00
enum
{
	RESPONSE_PULSE_LOW_54us_ERROR = 0x01,
	RESPONSE_PULSE_HIGH_85us_ERROR = 0x02,
	RESPONSE_PULSE_RECEIVED = 0x03
};

#define DATA_BIT_MASK 0x04
enum
{
	DATA_BIT_BEFORE_LOW_PULSE_ERROR = 0x05,
	DATA_BIT_ERROR = 0x06,
	DATA_BIT_OK = 0x07,
};

#define DATA_PULSE_MASK 0x08
enum
{
	DATA_PULSE_CHECKSUM_ERROR = 0x09,
	DATA_PULSE_OK = 0x010
};


void Gpio_input_pushpull_conf_dht11pin();
void Gpio_outout_pushpull_conf_dht11pin();
void timer3_init_dht11();
void GB_dht11_init();
int GB_dht11_find_response();
int GB_dht11_receivedht();
void GB_dht11();
uint8_t GB_dht11_hum();
float GB_dht11_temp();


#endif /* GB_DHT11_H_ */
