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
#include <stdint.h>
#include <stdio.h>
#include "gb_gpio.h"
#include "gb_i2c.h"
#include "gb_rcc.h"
#include "GB_UART.h"
#include "gb_t_delay.h"
#include "gb_ina219.h"
#include "stm32f1xx.h"
#include "gb_ssd1306.h"

//7 bit slave address is 0b0111100(0x3C)

/*
 * #if !defined(__SOFT_FP__) && defined(__ARM_FP)
 * #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
 * #endif
*/

uint8_t Character[8] =
{
		0b00000,
		0b11111,
		0b00100,
		0b01010,
		0b10001,
		0b00000,
		0b00000,
		0b00000
};
int main(void)
{
	system_clk();
	timer_initialise();
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
	//Gpio_output_pushpull_conf();

	GB_uart_init2();
	GB_printString2("Bus Voltage: ");

	gb_i2c_master_init();
	ssd1306_init();
	ssd1306_clear(GB_SSD1306_COLOR_BLACK);
	ssd1306_update_data();

    /* Loop forever */
	while(1)
	{


		ssd1306_GotoXY(123,0);
		ssd1306_print_char('K',GB_SSD1306_COLOR_WHITE);
		ssd1306_update_data();
		delay_ms(1);


		ssd1306_GotoXY(40,5);
		ssd1306_print_string("Kunal",GB_SSD1306_COLOR_WHITE);
		ssd1306_update_data();
		delay_ms(1000);

		ssd1306_GotoXY(65,5);
		ssd1306_print_string("gettobyte",GB_SSD1306_COLOR_WHITE);
		ssd1306_update_data();
	    delay_ms(1000);

	    ssd1306_GotoXY(78,5);
	   	ssd1306_float(10.78,GB_SSD1306_COLOR_WHITE);
	    ssd1306_update_data();
	   	delay_ms(1000);

	}
}
