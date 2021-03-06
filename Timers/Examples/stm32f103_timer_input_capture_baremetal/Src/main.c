/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "gb_gpio.h"
#include "gb_rcc.h"
#include "GB_UART.h"
#include "gb_t_delay.h"
#include "GB_millis_systick.h"
#include "stm32f1xx.h"
#include "gb_timer_input_capture.h"

/*
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
*/



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
/*
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
*/

uint32_t IC1Value = 0;
uint32_t IC2Value = 0;
float Frequency = 0;
float Duty = 0;
float Period = 0;
float Width = 0;
void TIM4_IRQHandler(void)
{
	if(TIM4->SR & TIM_SR_CC1IF) //capture comapre event 1
	{
		if(TIM4->DIER & TIM_DIER_CC1IE) //cross check that which source of interrupt is configured
		{
			if((TIM4->CCMR1 & TIM_CCMR1_CC1S) != 0) //cross check if input capture interrupt is thier if input capture event is their
			{
				IC1Value = TIM4->CCR1;
				if(IC1Value !=0)
				{
					IC2Value = TIM4->CCR2;
					Duty = (IC2Value*100)/IC1Value;
					Frequency = 50000/IC1Value;
					//Frequency = 1/IC1Value;
					Period = 1000/Frequency;//(Perid is in miliseconds)
					Width = Period/2;

				}
			}
		}
	}
}

int main(void)
{
	  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
		AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
	    systick_millis(32000);
	    system_clk();
		timer_initialise();
		GB_uart_init1();  //terminal
		GB_printString1("Timer input capture\n");
		gb_t4_input_capture();

		__NVIC_SetPriority (TIM4_IRQn, 1);
		__NVIC_EnableIRQ(TIM4_IRQn);


		while(1)
		{
			GB_printString1("Frequency: ");
			GB_float_value1(Frequency);
			GB_uart_newline1();


			GB_printString1("Duty: ");
			GB_decimel1(Duty);
			GB_uart_newline1();

			GB_printString1("Period: ");
			GB_float_value1(Period);
			GB_uart_newline1();

			GB_printString1("Width: ");
			GB_float_value1(Width);
			GB_uart_newline1();

			delay_ms(1000);
		}
}
