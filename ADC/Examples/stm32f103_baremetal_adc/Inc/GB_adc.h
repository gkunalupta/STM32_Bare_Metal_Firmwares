/*
 * GB_adc.h
 *
 *  Created on: 18-Sep-2021
 *      Author: kunal
 */

#ifndef GB_ADC_H_
#define GB_ADC_H_

#include "stm32f1xx.h"
#include <string.h>


void adc_init();
uint16_t adc_convert_START();

#endif /* GB_ADC_H_ */
