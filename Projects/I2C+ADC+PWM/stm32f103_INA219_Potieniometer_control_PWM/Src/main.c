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
#include "GB_adc.h"
#include "gb_t_delay.h"
#include "GB_UART.h"
#include "gb_ina219.h"
#include <gb_i2c.h>
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
uint16_t adc = 000;
uint32_t map_value = 0;

/**********************	STM32 Timer PWM device driver ************************************
 **********************Subscribe to Gettobyte ***********************************************
 *********************** Kunal Gupta ******************************************************
 */
int main(void)
{

	system_clk();
    pwm_init();
    SystemCoreClockUpdate();
 	//int a = SystemCoreClock ;
    adc_init();

  GB_uart_init2();
  GB_printString2("ADC Driver");
  ina219_init();


	while(1)
	{


	//TIM2->CCR1 = 20001;
 //adc = adc_convertPOL_START();
//GB_decimel2(adc);
//delay_ms(1000);
  //GB_printString2("\n");
//TIM2->CCR1 = 30000;
	// GB_printString2("ADC Driver");

adc = adc_convertPOL_START();
	map_value = adc * 1.22;
TIM2->CCR1 = map_value;


//	GB_decimel2(adc);
//	GB_printString2("\n");
//	GB_decimel2(map_value);
//	GB_printString2("\n");

//TIM2->CCR1 = map_value;

GB_printString2("Bus Voltage: ");
		ina219_busvoltage();
		GB_printString2("V\n");

		GB_printString2("Shunt Voltage: ");
		ina219_shuntvoltage();
		GB_printString2("mV\n");

		GB_printString2("Shunt Current: ");
		ina219_shuntcurrent();
		GB_printString2("mA\n");

		GB_printString2("Load Power : ");
		ina219_Loadpower();
		GB_printString2("mW\n");


		GB_printString2("\n");
		GB_printString2("\n");
		GB_printString2("\n");
	 for (l = 0; l < 900000; l++);
	//delay_ms(10);
//	 for (dl = 0; dl < 40000; dl++)
//	{
//					    TIM2->CCR1 = dl;
//		             for (l = 0; l < 2000; l++);
//				 }
//
//				 for (dl = 40000; dl >0; dl--)
//				  {
//					TIM2->CCR1 = dl;
//				    for (l = 0; l < 2000; l++);
//				  }
//				 for (l = 0; l < 1000; l++);
//
	}
}

