/*
 * t_delay.c
 *
 *  Created on: Apr 23, 2021
 *      Author: kunal
 */

#include "stm32f1xx.h"
#include "gb_t_delay.h"


//***************************Timer delay functions*************************************
/* Timer Frequency(TF) = bus Clock(Fsysclk)/(Prescalor(TIM2PSC) +1)
      bus Clock = 32Mhz, TIM2PSC = 0, So TF = 32MHZ

 Timer time Period(TTP) = 1/timer frequency(TF) = 1/32MHz = 0.03125 us

          TIM2ARR = 32
 So, Time delay by timer = Timer Time Period(TTP) * Timer auto reload register(TIM2ARR) = 32 * 0.03125us = 1us
 *
 *
 * TTP = 1us
 * TIM2ARR = 0xffff
 * 1 us = 1 count
 * 1ms = 1000us
 * 1 sec = 1000ms
 */
 void timer_initialise()
{
	// Start by making sure the timer's 'counter' is off
	TIM2->CR1 &= ~(TIM_CR1_CEN);
	TIM2->SR &= ~(TIM_SR_UIF);

	//RESET THE TIMER2 BUS
	RCC->APB1RSTR |=  (RCC_APB1RSTR_TIM2RST);
	RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM2RST);


	//Enable Timer2 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;


    TIM2-> CR1 |= TIM_CR1_UDIS; // enable this bit so that no register value is updated
/**********************for example 1*******************************/
    //Timer prescaler value
    TIM2->PSC = 32;
    //TIM2->PSC = 0;
    //Timer Auto reload register value
    TIM2->ARR = 0xffff;
    //65535
    //TIM2->ARR = 32;

    TIM2-> CR1 &= ~TIM_CR1_UDIS; // disable this bit so that values assigned from Timer register can move to preload registers

   //Enable the update generation for updating the shadow register with preload register contents with new values
   TIM2->EGR |= TIM_EGR_UG;
   //Clear the UIF flag as that is set when UG bit is set in EGR to update the content of my register
   TIM2->SR &= ~(TIM_SR_UIF);
}
void delay_us(uint16_t us)
{
	TIM2->CR1 |= TIM_CR1_CEN;
	//TIM2-> CR1 |= TIM_CR1_UDIS; // enable this bit so that no register value is updated
	//TIM2->EGR |= TIM_EGR_UG; // SET UG bit to reinitailse the counter register with 0
	TIM2->CNT=0; // initialise counter register
	//while(!(TIM2->SR & (1<<TIM_SR_UIF)));
	//TIM2->SR &= ~(TIM_SR_UIF); //clear the update interrupt flag as this is set when CNT is reinitialsed with 0
	while(TIM2->CNT < us)
	{	//uart_tran_dec(TIM2->CNT);
	}
	//TIM2->CR1 &= ~TIM_CR1_CEN;

}
void delay_ms(uint16_t ms)
{
	for (uint16_t i=0; i<ms; i++)
	delay_us(1000);
}
void delay_sec(uint16_t secs)
{
	for (uint16_t i=0; i<secs; i++)
	delay_ms(1000);
}
void delay()
{
	TIM2->CR1 |= TIM_CR1_CEN;
	while(!(TIM2->SR & TIM_SR_UIF));
	TIM2->SR &= ~(TIM_SR_UIF);
}
//static int l =0;
void delayus(uint16_t us)
{
   int i =0;
   //TIM2->CR1 |= TIM_CR1_CEN;
   while(i++<us)
   {
	   TIM2->CR1 |= TIM_CR1_CEN;
	   	while(!(TIM2->SR & TIM_SR_UIF));
	   	TIM2->SR &= ~(TIM_SR_UIF);
	  // i++;
   }
   //TIM2->CR1 &= ~TIM_CR1_CEN;
}
//static int p=0;
void delayms(uint16_t ms)
{
   int i=0;
   while(i++<ms)
   {
	  delayus(1000);
     // i++;
   }
  // TIM2->CR1 &= ~TIM_CR1_CEN;
}
