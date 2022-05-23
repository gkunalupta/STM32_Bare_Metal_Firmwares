/*
 * GB_millis_systick.h
 *
 *  Created on: 31-Jul-2021
 *      Author: kunal
 */



#ifndef GB_MILLIS_SYSTICK_H_
#define GB_MILLIS_SYSTICK_H_

#include "stm32f1xx.h"


#define NVIC_PRIORITYGROUP_4         0x00000003U /*!< 4 bits for pre-emption priority
                                                   //   0 bits for subpriority */



//variable for millis counter
extern volatile uint32_t GB_counter;

//system_tick handler on reaching 0
void SysTick_Handler(void);


//equivalent to  HAL_GetTick(void)
uint32_t GB_millis();
void systick_millis(uint32_t clk_val);
#endif /* GB_MILLIS_SYSTICK_H_ */
