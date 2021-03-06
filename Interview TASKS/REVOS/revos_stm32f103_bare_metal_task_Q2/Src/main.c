/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include <stdint.h>

#include "gb_gpio.h"
#include "gb_rcc.h"
#include "GB_UART.h"
#include "gb_t_delay.h"
#include "stm32f1xx.h"
#include "stm32f1xx.h"


/*
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
*/

//Debug Exception and Monitor Control Register base address
#define DEMCR        			*((volatile uint32_t*) 0xE000EDFCU )

/* ITM register addresses */
#define ITM_STIMULUS_PORT0   	*((volatile uint32_t*) 0xE0000000 )
#define ITM_TRACE_EN          	*((volatile uint32_t*) 0xE0000E00 )


int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		//__io_putchar(*ptr++);
		 ITM_SendChar(*ptr++);
	}
	return len;
}

#define START_PULSE_ERRORS_MASK 0x00
enum
{
	START_PULSE_OK = 0x01,
	START_PULSE_200_MS_LOW_ERROR = 0x02,
	START_PULSE_20_MS_HIGH_ERROR = 0x03
};

#define DATA_PULSE_ERROR_MASK 0x04
enum
{
	DATA_BIT_PULSE_LOW_ERROR = 0x05,
	DATA_BIT_PULSE_HIGH_ERROR = 0x06
};
// Using TIMER3 as Time base UNIT
void timer3_init()
	 {
	 // Start by making sure the timer's 'counter' is off
	 	TIM3->CR1 &= ~(TIM_CR1_CEN);
	 	TIM3->SR &= ~(TIM_SR_UIF);

	 	//RESET THE TIMER3 BUS
	 	RCC->APB1RSTR |=  (RCC_APB1RSTR_TIM3RST);
	 	RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM3RST);


	 	//Enable Timer3 peripheral clock
	     RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;


	     TIM3-> CR1 |= TIM_CR1_UDIS; // enable this bit so that no register value is updated

	     //Timer prescaler value
	     TIM3->PSC = 122; //TF = 256000hz, TP = 3.906us, so Max_Delay = 255.590ms
	                      // So now will be using the TIM3 as TIME BASE UNIT for monitoring the time period of START PULSE and DATA PULSES
	     //Timer Auto reload register value
	     TIM2->ARR = 0xffff;

	     TIM2-> CR1 &= ~TIM_CR1_UDIS; // disable this bit so that values assigned from Timer register can move to preload registers

	    //Enable the update generation for updating the shadow register with preload register contents with new values
	    TIM2->EGR |= TIM_EGR_UG;
	    //Clear the UIF flag as that is set when UG bit is set in EGR to update the content of my register
	    TIM2->SR &= ~(TIM_SR_UIF);
	 }

uint32_t Detection_start_pulse()
{
	 /**** detection of start pulse *********/
	 TIM2->CNT=0; // initialise counter register
	 while(!(READ_BIT((GPIOA->IDR), (GPIO_IDR_IDR8)))) // While Start pulse is LOW
	 {
		 if(TIM3->CNT >= 51230) //wait for 200.10ms by staying in the loop, if COUNTER is less then 51230 s
			                    //so as to check that our start signal is low for approx 200 ms
		 {
			 return START_PULSE_200_MS_LOW_ERROR;  //If Start Signal is LOW for more then 200.03ms then their is error
		 }
	 }
	 TIM2->CNT=0; // initialise counter register
	 while(READ_BIT((GPIOA->IDR), (GPIO_IDR_IDR8)))  // While Start pulse is HIGH
	 {
		 if(TIM3->CNT >= 5148) //wait for 20.10ms by staying in the loop, if COUNTER is less then 5148
			                   //so as to check that our start signal is HIGH for approx 20 ms
		 {
		 	return START_PULSE_20_MS_HIGH_ERROR;  // If Start Signal is HIGH for more then 20.03ms then their is error
		 }
	 }

	 return START_PULSE_OK;
}


uint8_t gb_data [7]; // array to store data bits value
uint32_t GB_CNT_LOW = 0;
uint32_t GB_CNT_HIGH = 0;
int main(void)
{

	 system_clk();
	 timer_initialise();

	 //int  l;
	 //PA8 pin selected as input pin
	 Gpio_input_pushpull_conf();

	 void timer3_init();
    /* Loop forever */
	while(1)
  {

		TIM2->CR1 |= TIM_CR1_CEN; // Counter Enable

			 if(Detection_start_pulse())
			 {

		          /**** detection of data bits******/
			         for (int gb_data_byte = 0; gb_data_byte < 8; ++gb_data_byte)  // total 7 bytes(0-55 bits: so it is 7 bytes of data)
			           {
			            for(int gb_data_bit = 7; gb_data_bit >= 0; --gb_data_bit)   // each byte has 8 bits from MSB to LSB
			                {
			            	    //DATA_BIT_LOW_LEVEL_detection
			    	             TIM3->CNT=0; // initialise counter register
			    	             while(!(READ_BIT((GPIOA->IDR), (GPIO_IDR_IDR8)))) // while Data pulse is low
			    		          {
			    			       if(TIM3->CNT <= 10267) //wait for 40.10ms by staying in loop till the signal is LOW and
			    			    	                      //equating COUNTER value to variable GB_CNT_LOW
			    			    	                      // As Maximum for 40ms Data Bit will be LOW, as soon as Data PULSE changes to HIGH
			    			    	                      //it will come out of while
			    			        {
			    				     GB_CNT_LOW = TIM3->CNT;
			    			        }
			    			       else
			    			        {
			    				     return DATA_BIT_PULSE_LOW_ERROR;  //If Data Signal is LOW for more then 40.03ms then their is error
			    			        }
			    		          }


			    	           //BIT_HIGH_LEVEL
			    	           //void BIT_HIGH_LEVEL_DETECT(uint32_t gpio_pin_input_register, uint32_t gpio_pin, uint32_t cnt_high_value)

			    	           TIM3->CNT=0; // initialise counter register
			    	           while(READ_BIT((GPIOA->IDR), (GPIO_IDR_IDR8)))  //if Data Pulse is HIGH
			    		       {
			    			     if(TIM3->CNT <= 10267) //wait for 40.10ms by staying in loop till the signal is HIGH and
		    	                                        //equating COUNTER value to variable GB_CNT_HIGH
		    	                                        // As Maximum for 40ms Data Bit will be HIGH, as soon as Data PULSE changes to LOW
		    	                                        //it will come out of while
			    			     {
			    			 	   GB_CNT_HIGH = TIM3->CNT;
			    			      }
			    			     else
			    			      {
			    			     return DATA_BIT_PULSE_HIGH_ERROR;  //If Data Signal is HIGH for more then 40.03ms then their is error
			    			      }
			    		        }


			    	    if (( GB_CNT_LOW >= 10217) & (GB_CNT_HIGH >= 5095))          //10217 is 39.90ms, 5095 is 19.90ms : this signifies BIT 0
			    	    CLEAR_BIT(gb_data[gb_data_byte],gb_data_bit);
			    	    if (( GB_CNT_LOW >= 5095) & (GB_CNT_HIGH >= 10217))          //5095 is 19.90ms, 10217 is 39.90ms : this signifies bit 1
			    	    SET_BIT(gb_data[gb_data_byte],gb_data_bit);
			             }
			          }
			}

			 //Printing the 7 bytes(0-55 bits) of data from UART2
			    for(int i = 0; i<8; i++)
			    {
			    	GB_decimel2(gb_data[i]);
			    	 delay_ms(100);
			    }
  }
}
