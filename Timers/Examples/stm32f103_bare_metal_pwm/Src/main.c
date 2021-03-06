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

//#include "default.h"
#include "rcc.h"
#include "t_pwm.h"
#include "gpio.h"

/*
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
*/

#include "stm32f1xx.h"


char rx_buf[1025];
char tx_buf[10];
unsigned long dl;
unsigned l;


/**********************	STM32 Timer PWM device driver ************************************
 **********************Subscribe to Gettobyte ***********************************************
 *********************** Kunal Gupta ******************************************************
 */
int main(void)
{

	 system_clk();
     pwm_init();
     SystemCoreClockUpdate();
 	int a = SystemCoreClock ;

	while(1)
	{
		 for (dl = 0; dl < 20000; dl++)
					 {
					    TIM2->CCR1 = dl;
			             for (l = 0; l < 200; l++);
					 }

					 for (dl = 20000; dl >0; dl--)
					  {
						TIM2->CCR1 = dl;
					    for (l = 0; l < 200; l++);
					  }
					 for (l = 0; l < 1000; l++);

	}
}

