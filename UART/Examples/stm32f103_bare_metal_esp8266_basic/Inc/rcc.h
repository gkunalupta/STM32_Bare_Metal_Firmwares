/*
 * rcc.h
 *
 *  Created on: 23-Apr-2021
 *      Author: kunal
 */

#ifndef INC_RCC_H_
#define INC_RCC_H_

#include "stm32f1xx.h"

void  MCO_pin_conf();
void system_clk();

/*
//variable for millis counter
volatile uint32_t GB_counter = 0;

//system_tick handler on reaching 0
void SysTick_Handler(void)
{
  GB_counter++;    // equivalent to HAL_IncTick(void)
}


//equivalent to  HAL_GetTick(void)
uint32_t GB_millis()
{
  return GB_counter;
}
*/

#endif /* INC_RCC_H_ */
