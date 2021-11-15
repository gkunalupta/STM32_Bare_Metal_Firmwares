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





//void dma_channel_init()
//{
//	RCC->AHBENR |= RCC_AHBENR_DMA1EN; // enable the DMA clock from AHB
//	//DMA1->CPAR1 = // address od ADC1_DATA register has to be feed.
//
//	DMA1_
//}
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


// Function to initialize ADC for single channel and using regular group
void adc_init()
{

//*** adc clock and pin configuration *****/
	RCC->APB2ENR|= RCC_APB2ENR_ADC1EN ; // Enable RCC Clock for ADC1
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->CFGR|= RCC_CFGR_ADCPRE_DIV4; //ADC PRESCALOR SET TO 4


	GPIOA->CRL&= ~(GPIO_CRL_MODE1_0 | GPIO_CRL_MODE1_1);    //PA1 channel 1
	GPIOA->CRL&= ~(GPIO_CRL_CNF1_0 | GPIO_CRL_CNF1_1);


//***** adc peripheral configuration ******/
    ADC1->CR2&= ~ADC_CR2_ALIGN; // RIGHT aligned selected  TP BE CHECK
    ADC1->CR2|= (ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2);   //Externel  Triger source for Regular group
   ADC1->CR2&= ~ADC_CR2_CONT; // Single conversion mode selected, CONTINOUS MODE DISABLED if : CONT = 1 Continous conversion mode will be selected
  // ADC1->CR2|= ADC_CR2_CONT; // Continous mode selected


    ADC1->CR1 &= ~(ADC_CR1_SCAN); // SCAN DISABLED, if using scan mode then dma must be enabled
    ADC1->CR1 &= ~(ADC_CR1_JDISCEN | ADC_CR1_DISCEN); // Discontinous mode disabled for both injected and regular groups
    ADC1->CR1 &= ~(ADC_CR1_DISCNUM_2 | ADC_CR1_DISCNUM_1 | ADC_CR1_DISCNUM_2 ); // no channels are configured in discontinous way.                                                 // if  discontinous mode is enabled then number of
                                                       //conversions to be done by discontinous mode has to be configured
                                                      // DISNUM bits
    ADC1->CR1 &= ~(ADC_CR1_DUALMOD_0 | ADC_CR1_DUALMOD_1 | ADC_CR1_DUALMOD_2); // INDEPENDENT MODE SELECTED
    ADC1->CR1 &= ~(ADC_CR1_AWDEN | ADC_CR1_JAWDEN);  // Analog watchdog disabled for both groups: regular and ibnjected
    ADC1->SQR1 &= ~(ADC_SQR1_L_0 | ADC_SQR1_L_1 | ADC_SQR1_L_2 | ADC_SQR1_L_3); // 1 channel length is selected for regular group
   // ADC1->SQR1 |= (ADC_SQR1_L_3); // 2 channel length is selected for regular group


/***** adc channel configuratiion ****/
    /*
     * Number of ADC Channels to be used in regular group are configured by L bits of SQR1
     * Now Channel  number which we are using IN0-IN9, that channel number binary value had to be fed into
     * SQR[1-3] register bits depending on the sequence that we want for ADC Channel.
     *
     * Say if we have Selected 2 As length of ADC in SQR1 Register,and want to use 2 channels Channel 0 and 1 of ADC
     * then we will set bits in SQR3 for specifying the sequence of those two channels.
     *
     *
     */
   // ADC1->SQR3 |= ADC_SQR3_SQ1_0; //1 ST SEQUENCE IN REGULAR GROUP IS CONFIGURED FOR ADC CHANNEL 1(PA1)
                                  //As 1 in 4 bit binary is 0001, and SQR3 register is divided into 4 bits
                                  //group(SQ6,SQ5,SQ4,etc) for configuring the conversion sequence
                                  //(SQx: x being the sequence of conversion)with channel number
                                  // sp we are using Channel 1 as first Sequence(SQ1) and 1 has binary value of
                                  //of 0001. Thus 0th bit of SQ1 is set as 1.

    //ADC1->SQR3 &= ~ADC_SQR3_SQ1; //2nd sequence in regular grp is configired for adc channel1


  //  ADC1->SQR3 &= ~(ADC_SQR3_SQ2); //2nd SEQUENCE IN REGULAR GROUP IS CONFIGURED FOR ADC CHANNEL 0(PA0)
                                  //As 0 in 4 bt binary is 0000, and we are using Channel 0 as Second Sequence(SQ2)
                                  // Thus all the bits of SQ2 is set as 1

    ADC1->SQR3 |= (ADC_SQR3_SQ2_0);//1 ST SEQUENCE IN REGULAR GROUP IS CONFIGURED FOR ADC CHANNEL 0




    ADC1->SMPR2&= ~ADC_SMPR2_SMP0; // ADC channel:0 Sample Time selected as 1.5 cycles  // TOBE CHECKED
    //ADC1->SMPR2&= ~ADC_SMPR2_SMP1; // ADC channel:1 Sample Time selected as 1.5 cycles  // TOBE CHECKED

}

/*
 * This Function start The ADC Conversion and We wait to get the ADC value by Polling, and then return the ADC Value
 */
uint16_t adc_convertPOL_START()
{
	uint16_t adc_data;
	ADC1->CR2&= ~ADC_CR2_ADON; // ADC disabled first and voltage regulator shuld be enabled before enabling corresponing ADC
	ADC1->CR2|= ADC_CR2_ADON; //ADC Enabled, before enabling should check that adc is disabled.

	delay_us(2); // wait for 2 us

	ADC1->SR &= ~ADC_SR_EOC; //Clear regular group conversion flag  (To ensure of no unknown state from potential previous ADC operations) */
	 ADC1->CR2|= ADC_CR2_EXTTRIG; // externel event trigger for conversion is enabled , always enabled
	 ADC1->CR2|= ADC_CR2_SWSTART; // Start ADC Conversion of regular group

	while(!(ADC1->SR & ADC_SR_EOC)); // wait till a group channel converstion has completed
	                                 //clear the EOC bit by reading DR register
	adc_data = ADC1->DR;
    return adc_data;


}

// Function for Initializing ADC for multiple channels by using DMA
void adc_init_multichannel()
{

//*** adc clock and pin configuration *****/
	RCC->APB2ENR|= RCC_APB2ENR_ADC1EN ; // Enable RCC Clock for ADC1
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->CFGR|= RCC_CFGR_ADCPRE_DIV4; //ADC PRESCALOR SET TO 4


	GPIOA->CRL&= ~(GPIO_CRL_MODE1_0 | GPIO_CRL_MODE1_1);    //PA1 channel 1
	GPIOA->CRL&= ~(GPIO_CRL_CNF1_0 | GPIO_CRL_CNF1_1);

	GPIOA->CRL&= ~(GPIO_CRL_MODE0_0 | GPIO_CRL_MODE0_1);    //PA0 channel 0
	GPIOA->CRL&= ~(GPIO_CRL_CNF0_0 | GPIO_CRL_CNF0_1);
//***** adc peripheral configuration ******/
    ADC1->CR2&= ~ADC_CR2_ALIGN; // RIGHT aligned selected  TP BE CHECK
    ADC1->CR2|= (ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2);   //Externel  Triger source for Regular group
   ADC1->CR2 |= ADC_CR2_CONT; // CONTINOUS MODE ENABLED if : CONT = 1 Continous conversion mode will be selected
  // ADC1->CR2|= ADC_CR2_CONT; // Continous mode selected


    ADC1->CR1 |= (ADC_CR1_SCAN); // SCAN ENABLED, if using scan mode then dma must be enabled
    ADC1->CR2 |= ADC_CR2_DMA; //ENABLE THE DMA


    ADC1->CR1 &= ~(ADC_CR1_JDISCEN | ADC_CR1_DISCEN); // Discontinous mode disabled for both injected and regular groups
    ADC1->CR1 &= ~(ADC_CR1_DISCNUM_2 | ADC_CR1_DISCNUM_1 | ADC_CR1_DISCNUM_2 ); // no channels are configured in discontinous way.                                                 // if  discontinous mode is enabled then number of
                                                       //conversions to be done by discontinous mode has to be configured
                                                      // DISNUM bits
    ADC1->CR1 &= ~(ADC_CR1_DUALMOD_0 | ADC_CR1_DUALMOD_1 | ADC_CR1_DUALMOD_2); // INDEPENDENT MODE SELECTED
    ADC1->CR1 &= ~(ADC_CR1_AWDEN | ADC_CR1_JAWDEN);  // Analog watchdog disabled for both groups: regular and ibnjected
   // ADC1->SQR1 &= ~(ADC_SQR1_L_0 | ADC_SQR1_L_1 | ADC_SQR1_L_2 | ADC_SQR1_L_3); // 1 channel length is selected for regular group
   //ADC1->SQR1 |= (ADC_SQR1_L_3); // 2 channel length is selected for regular group
    ADC1->SQR1 |= (ADC_SQR1_L_1); // 3 channel length is selected

   ADC1->CR2 |= (ADC_CR2_TSVREFE);//INTERNAL TEMPERATURE SENSOR ENABLED

/***** adc channel configuratiion ****/
    /*
     * Number of ADC Channels to be used in regular group are configured by L bits of SQR1
     * Now Channel  number which we are using IN0-IN9, that channel number binary value had to be fed into
     * SQR[1-3] register bits depending on the sequence that we want for ADC Channel.
     *
     * Say if we have Selected 2 As length of ADC in SQR1 Register,and want to use 2 channels Channel 0 and 1 of ADC
     * then we will set bits in SQR3 for specifying the sequence of those two channels.
     *
     *
     */
   // ADC1->SQR3 |= ADC_SQR3_SQ1_0; //1 ST SEQUENCE IN REGULAR GROUP IS CONFIGURED FOR ADC CHANNEL 1(PA1)
                                  //As 1 in 4 bit binary is 0001, and SQR3 register is divided into 4 bits
                                  //group(SQ6,SQ5,SQ4,etc) for configuring the conversion sequence
                                  //(SQx: x being the sequence of conversion)with channel number
                                  // sp we are using Channel 1 as first Sequence(SQ1) and 1 has binary value of
                                  //of 0001. Thus 0th bit of SQ1 is set as 1.


    ADC1->SQR3 &= ~ADC_SQR3_SQ1; //1st sequence in regular grp is configired for adc channel0
    ADC1->SQR3 |= (ADC_SQR3_SQ2_0);//2nd SEQUENCE IN REGULAR GROUP IS CONFIGURED FOR ADC CHANNEL 1
   ADC1->SQR3 |= (ADC_SQR3_SQ3_4);  //3rd seqyencxe in regular grp is configured for adc channel 16

  //  ADC1->SQR3 &= ~(ADC_SQR3_SQ2); //2nd SEQUENCE IN REGULAR GROUP IS CONFIGURED FOR ADC CHANNEL 0(PA0)
                                  //As 0 in 4 bt binary is 0000, and we are using Channel 0 as Second Sequence(SQ2)
                                  // Thus all the bits of SQ2 is set as 1


    ADC1->SMPR2 |= ADC_SMPR2_SMP0_1; // ADC channel:0 Sample Time selected as 1.5 cycles  // TOBE CHECKED
    ADC1->SMPR2 |= ADC_SMPR2_SMP1_1; // ADC channel:0 Sample Time selected as 1.5 cycles  // TOBE CHECKED
    ADC1->SMPR1 |= ADC_SMPR1_SMP16_1; // ADC channel:0 Sample Time selected as 1.5 cycles  // TOBE CHECKED

}

//Function for Initializing DMA Channel Peripheral,Memory and Length registers
void dma_for_adc(uint32_t srcadd, uint32_t destadd, uint16_t datasize)
{

    DMA1_Channel1->CCR &= ~DMA_CCR_EN; // DIABLE THE DMA CHANNEL 1 BEFORE CONFIGURING IT

    DMA1_Channel1->CNDTR = datasize; // No of data bytes to be transfered

    DMA1_Channel1->CPAR = srcadd; // as source address is ADC data Register, so as EOC bit in ADC is cleared as data is tranferred from ADC_data register to specified memory variable address

    DMA1_Channel1->CMAR = destadd; // Memory address of destination variable

    DMA1_Channel1->CCR |= DMA_CCR_EN; //enable the DMA

}

//Function to initialize the DMA Channel parameters
void dma_init(uint32_t srcadd, uint32_t destadd, uint16_t datasize)
{
	RCC->AHBENR |=RCC_AHBENR_DMA1EN;

	DMA1_Channel1->CCR &= ~DMA_CCR_EN; // DIABLE THE DMA CHANNEL 1 BEFORE CONFIGURING IT

    DMA1_Channel1->CCR &= ~DMA_CCR_PL; // channel 1 priorty set to very high

    DMA1_Channel1->CCR |= (DMA_CCR_MSIZE_0); // memory size transfer set as 16 bits

    DMA1_Channel1->CCR |= (DMA_CCR_PSIZE_0); // peripheral size transfer set as 16 bits

    DMA1_Channel1->CCR |= DMA_CCR_MINC; // memory address incremented automatically;

    DMA1_Channel1->CCR |= DMA_CCR_CIRC; //circular mode enabled

    DMA1_Channel1->CCR &= ~DMA_CCR_DIR; // read from peripheral

    DMA1_Channel1->CCR |= DMA_CCR_TCIE | DMA_CCR_HTIE; // full transfer and transfer interrupt enabled

	__NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	__NVIC_SetPriority (DMA1_Channel1_IRQn, 0);

	dma_for_adc(srcadd,destadd,datasize);   // initilise dma for adc1



}


/*
 * This Function start The ADC Conversion and We wait to get the ADC value by Polling, and then return the ADC Value
 */
void dma_adc_convert()
{
	// Enable the adc
	ADC1->CR2|= ADC_CR2_ADON; //ADC Enabled, before enabling should check that adc is disabled.

	//Start the ADC conversion
	ADC1->CR2|= ADC_CR2_EXTTRIG; // externel event trigger for conversion is enabled , always enabled
	ADC1->CR2|= ADC_CR2_SWSTART; // Start ADC Conversion of regular group

}
