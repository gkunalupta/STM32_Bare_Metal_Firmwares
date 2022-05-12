/*
 * gb_dht11.c
 *
 *  Created on: Oct 28, 2021
 *      Author: kunal
 */


#include "stm32f1xx.h"
#include "gb_dht11.h"
#include "GB_uart.h"




//USING GPIO  pb0  PIN
void Gpio_input_pushpull_conf_dht11pin()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	 GPIOB->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
	GPIOB->CRL &= ~(GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0); // MODE IS SELECTED AS INPUT

	GPIOB->CRL |= (GPIO_CRL_CNF0_1); // CONFIGURED AS INPUT PUSHPULL
	GPIOB->CRL &= ~(GPIO_CRL_CNF0_0);
}

void Gpio_outout_pushpull_conf_dht11pin()
{
	     RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
		 GPIOB->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);

		 //CONFIGURE GPIO PIN MODE AS OUTPUT MAX SPEED 50MHZ
		 GPIOB->CRL |= ( GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);

		 //CONFIGURE GPIO OUTPUT MODE AS OUTPUT PUSH PULL
		 GPIOB->CRL |= ~(GPIO_CRL_CNF0_1 | GPIO_CRL_CNF0_0);
}



void timer3_init_dht11()
	 {
	 // Start by making sure the timer's 'counter' is off
	 	TIM3->CR1 &= ~(TIM_CR1_CEN);
	 	TIM3->SR &= ~(TIM_SR_UIF);

	 	//RESET THE TIMER3 BUS
//	 	RCC->APB1RSTR |=  (RCC_APB1RSTR_TIM3RST);
//	 	RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM3RST);
	 	//Enable Timer3 peripheral clock
	    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	    TIM3-> CR1 |= TIM_CR1_UDIS; // enable this bit so that no register value is updated
	    TIM3-> PSC = 16; // TF = 2MHZ,TP = 5*10^-7, MAX DELAY = 32.76MS
	    //Timer Auto reload register value
	    TIM3->ARR = 0xffff;
	    TIM3-> CR1 &= ~TIM_CR1_UDIS; // disable this bit so that values assigned from Timer register can move to preload registers
	    //Enable the update generation for updating the shadow register with preload register contents with new values
	    TIM3->EGR |= TIM_EGR_UG;
	    //Clear the UIF flag as that is set when UG bit is set in EGR to update the content of my register
	    TIM3->SR &= ~(TIM_SR_UIF);

	    TIM3->CR1 |= TIM_CR1_CEN; // Counter Enable
	 }


void GB_dht11_init()
{
	timer3_init_dht11(); // initilse timer 3 at 2mhz for monitoring the time response for 1 -wire communication with dht11
	Gpio_outout_pushpull_conf_dht11pin();  //Set pin as output so that start signal can be send
	CLEAR_BIT(GPIOB->ODR, GPIO_ODR_ODR0);  // Low pulse is send as Start signal
	TIM3->CNT=0; // initialise counter register
	while(TIM3->CNT <=33999);  // Start signal of 18ms delay low signal is sent
	SET_BIT(GPIOB->ODR, GPIO_ODR_ODR0);   //High pulse is set to receive response from sensor
	TIM3->CNT=0; // initialise counter register
	while(TIM3->CNT <=75);  //wait for 42us to receive response
	Gpio_input_pushpull_conf_dht11pin();

}
//Now we have to check response given by DHT11 pin ..
int GB_dht11_find_response()
{
	TIM3->CNT=0;
	//sensor gives response of start signal by pulling the pin low/high for 54us
	while(!(READ_BIT((GPIOB->IDR), (GPIO_IDR_IDR0))))  //first response signal is low
	{
	if(TIM3->CNT >=179) // While sensor pull the pin low for 54us
	    {
		return RESPONSE_PULSE_LOW_54us_ERROR;
	    }

	}

	TIM3->CNT=0; // initialise counter register
	while((READ_BIT((GPIOB->IDR), (GPIO_IDR_IDR0))))   //Second response signal is high
	{

	if(TIM3->CNT >=179)//  While sensor pull the pin HIGH for 85us
	   {
	     return RESPONSE_PULSE_HIGH_85us_ERROR;
	    }


	}

	return RESPONSE_PULSE_RECEIVED;
}

/*
now sensor sends data and Pin is always in Input state for this phase
now data signal is received in the pack of 5 : each pack having 8 bits
each data bit is of HIGH pulse : 70us is read as 1
                                 26-28us is read as 0 bit
Each data Bit is followed by a Low Pulse of 54us

The output given out by the data pin will be in the order of
8bit humidity integer data + 8bit the Humidity decimal data
+8 bit temperature integer data + 8bit fractional temperature data +
8 bit parity bit
*/
uint8_t gb_data [5];
//int dht11_receivedht(uint8_t *x)
int GB_dht11_receivedht()
{
	 uint8_t  gb_check;
	 volatile uint8_t gb_cnt = 0;

		for (int gb_z = 0; gb_z < 5; ++gb_z)
		{
	        for(int gb_j = 7; gb_j >= 0; --gb_j)
	        {

	             /* First there is always a 50µs low period */
	        	TIM3->CNT=0; // initialise counter register
	        	while(!(READ_BIT((GPIOB->IDR), (GPIO_IDR_IDR0))))
	        	{
	        		if(TIM3->CNT >=107) ////Wait for 54us low pulse
	        		{
	        			GB_decimel1(TIM3->CNT);
	                    return DATA_BIT_BEFORE_LOW_PULSE_ERROR;
	        		}
	        	}

	       	 /* Then the data signal is sent. 26 to 28µs (ideally)
	            indicate a low bit, and around 70µs a high bit */
	        	TIM3->CNT=0; // initialise counter register
	        	while((READ_BIT((GPIOB->IDR), (GPIO_IDR_IDR0)))) //if signal is HIGH
	        	{
	        		if(TIM3->CNT <= 149)  // data signal is high for max 80us (as for 0 it is 26-28us and for 1 it is 70us)
	        		{gb_cnt = TIM3->CNT;    // So for 149 , 80us is generated to waiting for maximum time.
	        		//GB_decimel1(TIM3->CNT);
	        		}
	        		else
	        			{//GB_decimel1(TIM3->CNT);
	        			return DATA_BIT_ERROR;}
	        	}


	        	 /* Store the value now so that the whole checking doesn't
	        	             move the TCNT0 forward by too much to make the data look
	        	             bad */

	        	if ((gb_cnt >= 39) & (gb_cnt <= 69))          //20us to 35 us
	        	{ CLEAR_BIT(gb_data[gb_z],(1<<gb_j)); //GB_decimel1(gb_cnt);//GB_printString1("\n");

	        	}

	        	else if ((gb_cnt >= 119) & (gb_cnt <= 149))     // 60us to 80us
	        	{SET_BIT(gb_data[gb_z],(1<<gb_j));//GB_decimel1(gb_cnt);GB_printString1("\n");
	        	}

	        	else
	        	return DATA_BIT_OK;
	       }
      }
		gb_check = (gb_data[0] + gb_data[1] + gb_data[2] + gb_data[3]) & 0xFF;
		if (gb_check != gb_data[4]) return DATA_PULSE_CHECKSUM_ERROR;

//		for(int w = 0; w <= 4; w++)
//		{
//			GB_decimel1(gb_data[w]);
//			GB_printString1("\n");
//		}

		//return 4;
		return DATA_PULSE_OK;
}

void GB_dht11()
{
	GB_dht11_init();
	//GB_dht11_find_response();
	GB_decimel1(GB_dht11_find_response());
	GB_decimel1(GB_dht11_receivedht());
	//GB_dht11_receivedht();

	GB_printString1("\n");

}
uint8_t GB_dht11_hum()
{
	return (gb_data[0] + (0.1*gb_data[1]));

}

float GB_dht11_temp()
{
	//return ('78.09');
	return (gb_data[2] + (0.100*gb_data[3]));
}

