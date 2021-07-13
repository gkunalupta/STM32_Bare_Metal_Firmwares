/*
 * uart.c
 *
 *  Created on: Mar 11, 2021
 *      Author: kunal
 */



#include "uart.h"

//************************STM32- UART2 functions*******************//
//PA3- RX, PA2-TX
void uart2_pin_conf()
{
	// enable bus clock
	 RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	 RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

     //TX pin as alternate push pull
	 /* PA2 as output*/
	 GPIOA->CRL |= (GPIO_CRL_MODE2_1 | GPIO_CRL_MODE2_0);
	 GPIOA->CRL |= (GPIO_CRL_CNF2_1);
	 GPIOA->CRL &= ~(GPIO_CRL_CNF2_0);

	 /*
	   * PIN PA3 is the RX pin and it has to be set to input &FLOATING
	   * this is the rest value of the pin so we dont do anything to it
	   *
	   * */
}

void uart_boud_rate()
{
	  //uint16_t uartdiv = SystemCoreClock / 9600;
	//usartdiv = 208.33
	//0.33*16 = div_frac = d5.28 = 0x5
	//d208 = 0xd0
	//USART2->BRR =  ( ( uartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos );
	//USART2->BRR =  ( ( uartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) ;
     USART2->BRR = 0xd05;
}
void uart_init()
{

	uart2_pin_conf();
	//Alternate pin register
	/* no remap rx tx pins, for UART at pa2(tx) and pa3(rx)*/
    USART2->CR1 = 0x0000;
	//enable UART and configure stop byte
	USART2->CR1 |= USART_CR1_UE;
	USART2->CR1 &= ~USART_CR1_M;
	//enable transmitter
	USART2->CR1 |= USART_CR1_TE;
	//enable receiver
	USART2->CR1 |= USART_CR1_RE;
    uart_boud_rate();

}
void uart_tran_byte(uint8_t byte)
{
	while(!(USART2->SR & USART_SR_TXE));
	USART2->DR = byte;

}
uint16_t uart_rec_byte()
{
	while(!(USART2->SR & USART_SR_RXNE));
	return USART2->DR;
}
void uart_tran_string(const char *myString)      /////to print any string
{
	while (*myString)
	uart_tran_byte(*myString++);
	while(!(USART2->SR & USART_SR_TC));

}
void uart_tran_bin(uint32_t val)
{
	int8_t ptr;
	for(ptr=31;ptr>=0;ptr--)
	{
		if ((val & (1<<ptr))==0)
		{
			 uart_tran_byte('0');
		}
		else
		{
			 uart_tran_byte('1');
		}
	}
}
void uart_tran_dec(uint32_t val)
{
	unsigned char buf[5];
	int8_t ptr;
	for(ptr=0;ptr<5;++ptr)
	{
		buf[ptr] = (val % 10) + '0';
		val /= 10;
	}
	for(ptr=4;ptr>0;--ptr)
	{
		if (buf[ptr] != '0')
		break;
	}
	for(;ptr>=0;--ptr)
	{
		 uart_tran_byte(buf[ptr]);
	}
}
void newlin_uart()
{
	uart_tran_byte('\n');
}
void uart_rec(char *buff)
{
	int i=0;
	uint16_t myValue;
	do
	{

		myValue = uart_rec_byte();
		if(myValue!='\n')
		{
			buff[i]=myValue;
			i++;
		}    else
		{
			buff[i]='\0';
			break;
		}

	}
	while(!(USART2->SR & USART_SR_RXNE));
}

