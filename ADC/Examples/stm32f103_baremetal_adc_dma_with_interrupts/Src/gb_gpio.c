/*
 * gpio.c
 *
 *  Created on: Apr 23, 2021
 *      Author: kunal
 */

#include "stm32f1xx.h"
#include "gb_gpio.h"


/************* gpio functions******************
on board led output push pull PC13
*/
void Gpio_output_pushpull_conf()
{
	     RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
		 GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);

		 //CONFIGURE GPIO PIN MODE AS OUTPUT MAX SPEED 50MHZ
		 GPIOC->CRH |= ( GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0);

		 //CONFIGURE GPIO OUTPUT MODE AS OUTPUT PUSH PULL
		 GPIOC->CRH |= ~(GPIO_CRH_CNF13_1 | GPIO_CRH_CNF13_0);
}

