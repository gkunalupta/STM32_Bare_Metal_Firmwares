/*
 * t_pwm.c
 *
 *  Created on: Apr 23, 2021
 *      Author: kunal
 */

#include "stm32f1xx.h"
#include "t_pwm.h"

void T2C1_pin_conf()
{
	 RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	 GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
	 //CONFIGURE GPIO PIN MODE AS OUTPUT MAX SPEED 50MHZ
	 GPIOA->CRL |= ( GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);
	 //CONFIGURE GPIO OUTPUT MODE AS alternate function push-pull
	 GPIOA->CRL |= ((GPIO_CRL_CNF0_1) | ~(GPIO_CRL_CNF0_0));
}

/*
 * To Produce the  PWM signal :
Important Registers
Timer Counter register: CNT
AutoReload Register: Frequency: ARR: 200
Output Compare register: Duty Cycle: CCRX :130

--> Mode of Timer
--> Output pin polarity

1) Enable Timer and Gpio peripheral of channel
2) Enable ARR & CCR1 preload register
3) Select the channel and enable it

4) set up the ARR,CCR1.PSC

5)Set the mode of Timer and polarity of output pin

6) enable the counter

 */

void pwm_init()
{
	// Start by making sure the timer's 'counter' is off
		TIM2->CR1 &= ~(TIM_CR1_CEN);
		TIM2->SR &= ~(TIM_SR_UIF);

		//RESET THE TIMER2 BUS
		RCC->APB1RSTR |=  (RCC_APB1RSTR_TIM2RST);
		RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM2RST);

		//enable Timer2channel 1 gpio periperial
		T2C1_pin_conf();
		//Enable Timer2 peripheral clock
	    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	    //enable the preload for CCR1 register -OC1PE bits
	    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
	    //enable the preload for ARR register
	    TIM2->CR1 |= TIM_CR1_ARPE;
	    //Set channel1 as output for PWM mode
	    TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;
	    //enable the channel 1 of TIM2
	 	TIM2->CCER |= TIM_CCER_CC1E;
/******************* assigning values to ARR,PSC,CCR1***************/
	    TIM2-> CR1 |= TIM_CR1_UDIS; // enable this bit so that no register value is updated

	    //Timer prescaler value
	     TIM2->PSC = 32;
	     //Timer Auto reload register value - this decides the frequency of my signal
	     TIM2->ARR = 20000;
	     //timer output compare register for channel 1 upto which my counter will count: this decides the duty cycle of my signal
	     TIM2->CCR1 = 15000;

	     TIM2-> CR1 &= ~TIM_CR1_UDIS; // disable this bit so that values assigned from Timer register can move to preload registers
	     //Enable the update generation for updating the shadow register with preload register contents with new values
	      TIM2->EGR |= TIM_EGR_UG;
	      //Clear the UIF flag as that is set when UG bit is set in EGR to update the content of my register
	      TIM2->SR &= ~(TIM_SR_UIF);

/*** Setting the output mode of the output signal,its polarity and enabling the corresponding channel ********/

	      //This bits selects which output mode is selected :set the pwm mode 1 on channel1- OC1M bits
	      TIM2->CCMR1 |= (1<<6)|(1<<5);
	     // output signal polarity of channel 1 - active high
	      TIM2->CCER &= ~(TIM_CCER_CC1P);
	      //set the p
	      //enable the counter
	      TIM2->CR1 |= TIM_CR1_CEN;
}
void example_func()
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
