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
#include "hlw8012_baremetal.h"

/*
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
*/

#define UPDATE_TIME                     2000

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

#define CURRENT_RESISTOR                0.001
#define VOLTAGE_RESISTOR_UPSTREAM       ( 5 * 470000 ) // Real: 2280k
#define VOLTAGE_RESISTOR_DOWNSTREAM     ( 1000 ) // Real 1.009k


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

		hlw8012_config();

		 //Configures GPIO PIN PB8 as general purpose output
		 RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
			 GPIOB->CRH |= (GPIO_CRH_MODE12_0 | GPIO_CRH_MODE12_1);
			 GPIOB->CRH &= ~(GPIO_CRH_CNF12_0 | GPIO_CRH_CNF12_1);

		hlw8012_setResistors(CURRENT_RESISTOR, VOLTAGE_RESISTOR_UPSTREAM, VOLTAGE_RESISTOR_DOWNSTREAM);

		GB_printString1(" Default Current Multiplier");
		GB_decimel1(hlw8012_getcurrent_multiplier());

		GB_printString1(" Default Voltage Multiplier");
		GB_decimel1(hlw8012_getvoltage_multiplier());

		GB_printString1(" Default Power Multiplier");
		GB_decimel1(hlw8012_getpower_multiplier());

		hlw8012_calibrate();


		while(1)
		{

//	unsigned long last = GB_millis();	// Timestamp coming into function
////
////			  // This UPDATE_TIME should be at least twice the minimum time for the current or voltage
////			  // signals to stabilize. Experimentally that's about 1 second.
//	  if ((GB_millis() - last) > UPDATE_TIME) {
//       last = GB_millis();
//
//
//
			        GB_printString1("Current in AMPS: ");
			        GB_float_value1(hlw8012_getcurrent());
			        GB_uart_newline1();

			        GB_printString1("Voltage in Volts: ");
			       	GB_float_value1(hlw8012_getvoltage());
			       	GB_uart_newline1();


	 // }

//			GB_printString1("Frequency: ");
//			GB_float_value1(Frequency);
//			GB_uart_newline1();
//
//
//			GB_printString1("Duty: ");
//			GB_decimel1(Duty);
//			GB_uart_newline1();
//
//			GB_printString1("Period: ");
//			GB_float_value1(Period);
//			GB_uart_newline1();


		//	delay_ms(1000);
		}
}
