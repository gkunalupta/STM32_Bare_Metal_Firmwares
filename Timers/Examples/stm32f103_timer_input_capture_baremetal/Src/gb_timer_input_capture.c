/*
 * gb_timer_input_capture.c
 *
 *  Created on: May 6, 2022
 *      Author: kunal
 */

#include "stm32f1xx.h"
#include "gb_timer_input_capture.h"
//PB6
void T4C1_pin_conf()
{
	 RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	 //Configures GPIO PIN PB6 as Input
	 GPIOB->CRL &= ~(GPIO_CRL_MODE6);
	 //CONFIGURE GPIO PIN PB6 as floating input
	 GPIOB->CRL |= (GPIO_CRL_CNF6_0);
	 GPIOB->CRL &= ~(GPIO_CRL_CNF6_1);
}

void gb_t4_input_capture()
{
	// Start by making sure the timer's 'counter' is off
			TIM4->CR1 &= ~(TIM_CR1_CEN);
			TIM4->SR &= ~(TIM_SR_UIF);

			//RESET THE TIMER4 BUS
			RCC->APB1RSTR |=  (RCC_APB1RSTR_TIM4RST);
			RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM4RST);

			//Enable Timer4 peripheral clock
			RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

			//enable Timer4channel 1 gpio periperial
			T4C1_pin_conf();

//			__NVIC_SetPriority (TIM4_IRQn, 0);
//
//			__NVIC_EnableIRQ(TIM4_IRQn);

			//***********Timer base configuration******************
		    TIM4->CR1 |= TIM_CR1_ARPE; //enable the preload fpr arr register


		    //Timer prescaler value
		     TIM4->PSC = 640;
		     //Timer Auto reload register value - this decides the frequency of my signal
		     TIM4->ARR = 0xffff;

		     TIM4-> CR1 &= ~TIM_CR1_UDIS; // disable this bit so that values assigned from Timer register can move to preload registers
		     //Enable the update generation for updating the shadow register with preload register contents with new values
		     TIM4->EGR |= TIM_EGR_UG;
//		     //When UIF bit is set to 1, it means registers are updated and update interrupt is pending.
//		     while(!(TIM4->SR & TIM_SR_UIF));
//		     //Clear the UIF flag as that is set when UG bit is set in EGR to update the content of  register
//		   TIM4->SR &= ~(TIM_SR_UIF);


		   //********************************** TIMER Clock configuration ***************
		   TIM4->SMCR &= ~ (TIM_SMCR_SMS |TIM_SMCR_TS | TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE | TIM_SMCR_ETP);

		   //***********Timer base configuration******************
		    TIM4->CR1 |= TIM_CR1_ARPE; //enable the preload fpr arr register
		    //Timer prescaler value
			TIM4->PSC = 640;
			//Timer Auto reload register value - this decides the frequency of my signal
			TIM4->ARR = 0xfffffff;
			TIM4-> CR1 &= ~TIM_CR1_UDIS; // disable this bit so that values assigned from Timer register can move to preload registers
			//Enable the update generation for updating the shadow register with preload register contents with new values
			 TIM4->EGR |= TIM_EGR_UG;
			 //When UIF bit is set to 1, it means registers are updated and update interrupt is pending.
			 while(!(TIM4->SR & TIM_SR_UIF));
			 //Clear the UIF flag as that is set when UG bit is set in EGR to update the content of  register
			// TIM4->SR &= ~(TIM_SR_UIF);



			 //**********************Timer Slave configuration*****************
		     TIM4-> SMCR = 0x00;// reset the slave mode register
		     TIM4->SMCR |= TIM_SMCR_SMS_2; //slave mode selection: Reset Mode.
		     TIM4->SMCR |= (TIM_SMCR_TS_2 | TIM_SMCR_TS_0); //slave mode: trigger selection
		     /* Configure the trigger prescaler, filter, and polarity */


		     TIM4->DIER &= ~TIM_DIER_TIE; //trigger interrupt enable: triggered if UIF flag is set to 1
		     TIM4->DIER &= ~TIM_DIER_TDE; // trigger dma request disabled

		    //IC1 capture disabled
		    TIM4->CCER &= ~TIM_CCER_CC1E;
		    //CC1 channel is configures as input, IC1 is mapped tp TI1
            TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
            TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
            //IC1 capture polarity non-inverted: capture is done on rising edge of IC1.
            TIM4->CCER &= ~TIM_CCER_CC1P;
            TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC; // No prescaler, capture is done each time an edge is detected.
            TIM4->CCMR1 &= ~TIM_CCMR1_IC1F; // No input filter is needed, as input signal is stable so at each event capture can be done


            //IC2 capture disabled
           	 TIM4->CCER &= ~TIM_CCER_CC2E;
            //IC2 channel is configures as input, IC2 is mapped tp TI1
             TIM4->CCMR1 &= ~TIM_CCMR1_CC2S_0;
             TIM4->CCMR1 |= TIM_CCMR1_CC2S_1;
             //IC2 capture polarity inverted: capture is done on falling edge of IC2.
             TIM4->CCER |= TIM_CCER_CC2P;
             TIM4->CCMR1 &= ~TIM_CCMR1_IC2PSC; // No prescaler, capture is done each time an edge is detected.
             TIM4->CCMR1 &= ~TIM_CCMR1_IC2F; // No input filter is needed, as input signal is stable so at each event capture can be done


             //Master configuration
             TIM4->CR2 &= ~TIM_CR2_MMS; //reset the master mode selection
             TIM4->CR2 = 0x00;




             //Timer4_channel1 configuration
             TIM4->DIER |= TIM_DIER_CC1IE; //capture interrupt enable
             TIM4->CCER |= TIM_CCER_CC1E; // timer 4 channel1 configured for capture enabled
             TIM4->CR1 |= TIM_CR1_CEN; //enable the timer

//        	 //When UIF bit is set to 1, it means registers are updated and update interrupt is pending.
//        	 while(!(TIM4->SR & TIM_SR_UIF));
//        		//Clear the UIF flag as that is set when UG bit is set in EGR to update the content of  register
//             TIM4->SR &= ~(TIM_SR_UIF);
//             TIM4->SR &= ~(TIM_SR_UIF);
//             TIM4->CR1 &= ~TIM_CR1_CEN;
//             //TIM4->DIER |= TIM_DIER_CC1IE; //capture interrupt enable
//             TIM4->CR1 |= TIM_CR1_CEN; //enable the timer

             //Timer4_channel2 configuration
             TIM4->CCER |= TIM_CCER_CC2E;
             TIM4->CR1 |= TIM_CR1_CEN; //enable the timer
}
//
//uint32_t IC1Value = 0;
//uint32_t IC2Value = 0;
//uint32_t Frequency = 0;
//float Duty = 0;
//void TIMER4_IRQHandler(void)
//{
//	if(TIM4->SR & TIM_SR_CC1IF) //capture comapre event 1
//	{
//		if(TIM4->DIER & TIM_DIER_CC1IE) //cross check that which source of interrupt is configured
//		{
//			if((TIM4->CCMR1 & TIM_CCMR1_CC1S) != 0) //cross check if input capture interrupt is thier if input capture event is their
//			{
//				IC1Value = TIM4->CCR1;
//				if(IC1Value !=0)
//				{
//					IC2Value = TIM4->CCR2;
//					Duty = (IC2Value*100)/IC1Value;
//					Frequency = 1000000/IC1Value;
//				}
//			}
//		}
//	}
//}
int gb_a;
int gb_t4_calculate()
{
while(!(TIM4->SR & TIM_SR_CC1IF)); // Wait till edge is detetcted to capture the counter value
gb_a = TIM4->CCR1; // CC1IF flag is cleared by reading the CCR1 register
return gb_a;
}
