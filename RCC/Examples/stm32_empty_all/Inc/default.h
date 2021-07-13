#include "stm32f1xx.h"

void system_clk()
{
	//ENABLE HSE & WAIT FOR HSE TO BECOME READY
	RCC->CR |= RCC_CR_HSEON ;   // HSE IS 8MHZ
	while(!(RCC->CR & RCC_CR_HSERDY))
		 ;
	//configure flash prefetch and the latency related
	FLASH->ACR |= FLASH_ACR_PRFTBE|FLASH_ACR_LATENCY_1;
	// CONFIGURE PLL AND Buses(AHB,APB1,APB2)
	//PLL SOURCE: HERE  HSE IS USED AS SOURCE
	RCC->CFGR |= RCC_CFGR_PLLSRC;
	//HSE DEVIDER FOR PLL(IF HSE IS USED AS SOURCE FOR PLL) : NOT DIVIDING CLOCK
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE ;  // SO INPUT TO PLL IS 8MHZ
	//PLL MULTIPLIER: HERE I AM MULTIPLYING HSE OUTPUT WITH 4
	RCC->CFGR |= RCC_CFGR_PLLMULL4; //SO OUTPUT FROM PLL IS 32MHZ
	//BUS CLOCK CONFIGURE(APB1,APB2,AHB): NOT DIVIDING
	RCC->CFGR |= (RCC_CFGR_PPRE1_DIV1 | RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_HPRE_DIV1);  // all buses runs at 32MHZ
	//ENABLE THE PLL
	RCC->CR |= RCC_CR_PLLON;
	//WAIT FOR PLL TO SET
	while(!(RCC->CR & RCC_CR_PLLRDY))
			;

	 //ENABLE SYSTEMCLK AND WAIT
	 RCC->CFGR |= RCC_CFGR_SW_PLL;
	 while(!(RCC->CFGR & RCC_CFGR_SWS_PLL))
	 		 ;

	 MCO_pin_conf();
	 //CLOCK OUTPUT ON MCO PIN
	 RCC->CFGR |= RCC_CFGR_MCO_SYSCLK;
}

/* Timer Frequency(TF) = bus Clock(Fsysclk)/(Prescalor(TIM2PSC) +1)
 *
 * Timer time Period(TTP) = 1/timer frequency(TF)
 *
 * Time delay by timer = Timer Time Period(TTP) * Timer auto reload register(TIM2ARR)
 *
 * System Clock = 32Mhz, TIM2PSC = 32, So TF = 1MHZ
 *
 * TTP = 1us
 * TIM2ARR = 0xffff
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

    //Enable the update generation
    TIM2->EGR |= TIM_EGR_UG;

/**********************for example 1*******************************/
    //Timer prescaler value
    TIM2->PSC = 32;
    //Timer Auto reload register value
    TIM2->ARR = 0xffff; //65535

    TIM2->CR1 |= TIM_CR1_CEN;
    //while(!(TIM2->SR & (1<<TIM_SR_UIF)));


}

/* 1 us = 1 count
 * 1ms = 1000us
 * 1 sec = 1000ms
 */


//Using Pooling example1
void delay_us(uint16_t us)
{
	TIM2->CNT=0;
	while(TIM2->CNT < us);

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

