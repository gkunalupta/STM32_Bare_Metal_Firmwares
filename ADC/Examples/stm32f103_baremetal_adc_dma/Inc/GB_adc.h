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

/*
 * ADC Peripheral Initialisation
 * 1) Clock enable for ADC and ADC Channel PIN Configuration
 *
 * 2) ADC Peripheral Configurations
 * --> Configuration of Trigger Source , Regular Group/Injected Group,
 * --> Number of ADC Channels to be used
 * --> enabling and disabling of SCAN,DISCONTINOUS,CONTINOUS modes
 * --> Confguring ADC Modes, Analog WatchDog
 *
 * 3) ADC Channel configuration
 *   --> Sampling Time of ADC Channel and Sequence of Channels in Regular Groups
 */
void adc_init();
/*
 * This Function start The ADC Conversion and We wait to get the ADC value by Polling, and then return the ADC Value
 */
uint16_t adc_convertPOL_START();



void adc_init_multichannel();

void dma_init();

void dma_adc_convert(uint32_t srcadd, uint32_t destadd, uint16_t datasize);

void dma_for_adc(uint32_t srcadd, uint32_t destadd, uint16_t datasize);


#endif /* GB_ADC_H_ */
