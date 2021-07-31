/*
 * GB_millis_systick.c
 *
 *  Created on: 31-Jul-2021
 *      Author: kunal
 */

#include "GB_millis_systick.h"

#include "stm32f1xx.h"


void SysTick_Handler(void)
{
	//GB_printString1("gup\n");
  GB_counter++;    // equivalent to HAL_IncTick(void)
}


//equivalent to  HAL_GetTick(void)
uint32_t GB_millis()
{
	//GB_printString1("kun\n");
  return GB_counter;
}

void systick_millis(uint32_t clk_val)
{
	FLASH->ACR |= FLASH_ACR_PRFTBE|FLASH_ACR_LATENCY_1;
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	SysTick_Config(clk_val);
	NVIC_SetPriority(SysTick_IRQn,0);

}
