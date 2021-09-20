/*
 * GB_adc.c
 *
 *  Created on: 18-Sep-2021
 *      Author: kunal
 */


#include <GB_adc.h>
#include <string.h>
#include "stdio.h"
#include "stm32f1xx.h"
#include "system_stm32f1xx.h"
#include "gb_t_delay.h"

  #define ADC_STAB_DELAY_US               1U

void adc_init()
{

//*** adc clock and pin configuration *****/
	RCC->APB2ENR|= RCC_APB2ENR_ADC1EN ; // Enable RCC Clock for ADC1
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->CFGR|= RCC_CFGR_ADCPRE_DIV4; //ADC PRESCALOR SET TO 4


	GPIOA->CRL&= ~(GPIO_CRL_MODE1_0 | GPIO_CRL_MODE1_1);
	GPIOA->CRL&= ~(GPIO_CRL_CNF1_0 | GPIO_CRL_CNF1_1);

    RCC->CFGR|= RCC_CFGR_ADCPRE_DIV4; //ADC PRESCALOR SET TO 4

//***** adc peripheral configuration ******/
    ADC1->CR2&= ~ADC_CR2_ALIGN; // RIGHT aligned selected  TP BE CHECK
    ADC1->CR2|= (ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2);   //Externel  Triger source for Regular group
   ADC1->CR2&= ~ADC_CR2_CONT; // Single conversion mode selected, CONTINOUS MODE DISABLED if : CONT = 1 Continous conversion mode will be selected



    ADC1->CR1 &= ~(ADC_CR1_SCAN); // SCAN DISABLED
    ADC1->CR1 &= ~(ADC_CR1_JDISCEN | ADC_CR1_DISCEN); // Discontinous mode disabled for both injected and regular groups
    ADC1->CR1 &= ~(ADC_CR1_DISCNUM_2 | ADC_CR1_DISCNUM_1 | ADC_CR1_DISCNUM_2 ); // no channels are configured in discontinous way.                                                 // if  discontinous mode is enabled then number of
                                                       //conversions to be done by discontinous mode has to be configured
                                                      // DISNUM bits
    ADC1->CR1 &= ~(ADC_CR1_DUALMOD_0 | ADC_CR1_DUALMOD_1 | ADC_CR1_DUALMOD_2); // INDEPENDENT MODE SELECTED
    ADC1->CR1 &= ~(ADC_CR1_AWDEN | ADC_CR1_JAWDEN);  // Analog watchdog disabled for both groups: regular and ibnjected
    ADC1->SQR1 &= ~(ADC_SQR1_L_0 | ADC_SQR1_L_1 | ADC_SQR1_L_2 | ADC_SQR1_L_3); // 1 channel length is selected for regular group



/***** adc channel configuratiion ****/
    ADC1->SQR3 |= ADC_SQR3_SQ1_0; //1 ST SEQUENCE IN REGULAR GROUP IS CONFIGURED FOR adc CHANNEL 1.
    ADC1->SMPR2&= ~ADC_SMPR2_SMP1; // ADC channel:1 Sample Time selected as 1.5 cycles  // TOBE CHECKED


}
uint16_t adc_convert_START()
{
	uint16_t adc_data;
	// __IO uint32_t wait_loop_index = 0U;
	ADC1->CR2&= ~ADC_CR2_ADON; // ADC disabled first and voltage regulator shuld be enabled before enabling corresponing ADC
	ADC1->CR2|= ADC_CR2_ADON; //ADC Enabled, before enabling should check that adc is disabled.

	delay_us(2); // wait for 2 us


//	wait_loop_index = (ADC_STAB_DELAY_US * (SystemCoreClock / 1000000U));
//    while(wait_loop_index != 0U)
//    {
//      wait_loop_index--;
//    }
	ADC1->SR &= ~ADC_SR_EOC; //Clear regular group conversion flag  (To ensure of no unknown state from potential previous ADC operations) */
	 ADC1->CR2|= ADC_CR2_EXTTRIG; // externel event trigger for conversion is enabled , always enabled
	 ADC1->CR2|= ADC_CR2_SWSTART; // Start ADC Conversion of regular group
//	while(!(ADC1->SR & ADC_SR_STRT)); // wait till regular group conversion has started also start bit is cleared



	while(!(ADC1->SR & ADC_SR_EOC)); // wait till a group channel converstion has completed
	                                 //clear the EOC bit by reading DR register
	adc_data = ADC1->DR;
	//adc_data = ADC1->DR;
return adc_data;


}
