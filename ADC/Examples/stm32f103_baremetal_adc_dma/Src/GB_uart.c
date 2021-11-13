/*
 * uart.c
 *
 *  Created on: Mar 11, 2021
 *      Author: kunal
 */



#include <GB_uart.h>
#include <string.h>
#include "stdio.h"
/**************STM32 UART1 ******************/
//PA10- RX, PA9-TX
void GB_uart_pin_conf1()
{
	// enable bus clock
	 RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	 RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

     //TX pin as alternate push pull
	 /* PA9 as output*/
	 GPIOA->CRH |= (GPIO_CRH_MODE9_1 | GPIO_CRH_MODE9_0);
	 GPIOA->CRH |= (GPIO_CRH_CNF9_1);
	 GPIOA->CRH &= ~(GPIO_CRH_CNF9_0);

	 /*
	   * PIN PA10 is the RX pin and it has to be set to input &FLOATING
	   * this is the rest value of the pin so we dont do anything to it
	   *
	   * */
}

void GB_uart_boud_rate1()
{
	  //uint16_t uartdiv = SystemCoreClock / 16* 115200;
	//usartdiv = 0d17.36
	//DIV_Fraction = 0.36*16 = 0d5.76 = 0d6 (Nearest Whole number) = 0x6
	//DIV_Mantissa = 0d17 = 0x11
	//USART2->BRR =  ( ( uartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos );
	//USART2->BRR =  ( ( uartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) ;
    // USART1->BRR = 0x116;
	USART1->BRR = 0xd05;

}
void GB_uart_init1()
{

	GB_uart_pin_conf1();
	//Alternate pin register
	/* no remap rx tx pins, for UART at pa2(tx) and pa3(rx)*/
    USART1->CR1 = 0x0000;
	//enable UART and configure stop byte
	USART1->CR1 |= USART_CR1_UE;
	USART1->CR1 &= ~USART_CR1_M;

	 //enable RXNE interrupts on USART SIDE
	//USART1->CR1 |= USART_CR1_RXNEIE;

	//enable transmitter
	USART1->CR1 |= USART_CR1_TE;
	//enable receiver
	USART1->CR1 |= USART_CR1_RE;


    GB_uart_boud_rate1();


}
void GB_UART_TxChar1(uint32_t gb_byte)
{
	while(!(USART1->SR & USART_SR_TXE));
	USART1->DR = gb_byte;

}
uint16_t GB_UART_RxChar1()
{
	while(!(USART1->SR & USART_SR_RXNE));
	return USART1->DR;
}uint16_t GB_UART_RxChar2()
{
	while(!(USART2->SR & USART_SR_RXNE));
	return USART2->DR;
}
void GB_printString1(const char *gb_myString)      /////to print any string
{
	while (*gb_myString)
	GB_UART_TxChar1(*gb_myString++);
	while(!(USART1->SR & USART_SR_TC));

}
void GB_bit1(uint32_t gb_val)
{
	int8_t gb_ptr;
	for(gb_ptr=31;gb_ptr>=0;gb_ptr--)
	{
		if ((gb_val & (1<<gb_ptr))==0)
		{
			GB_UART_TxChar1('0');
		}
		else
		{
			GB_UART_TxChar1('1');
		}
	}
}
void GB_decimel1(uint32_t gb_val)
{
	unsigned char gb_buf[5];
	int8_t gb_ptr;
	for(gb_ptr=0;gb_ptr<5;++gb_ptr)
	{
		gb_buf[gb_ptr] = (gb_val % 10) + '0';
		gb_val /= 10;
	}
	for(gb_ptr=4;gb_ptr>0;--gb_ptr)
	{
		if (gb_buf[gb_ptr] != '0')
		break;
	}
	for(;gb_ptr>=0;--gb_ptr)
	{
		GB_UART_TxChar1(gb_buf[gb_ptr]);
	}
}
void GB_uart_newline1()
{
	GB_UART_TxChar1('\n');
}
void GB_uart_rec1(char *gb_buff)
{
	int gb_i=0;
	uint16_t gb_myValue;
	do
	{

		gb_myValue = GB_UART_RxChar2();
		if(gb_myValue!='\n')
		{
			gb_buff[gb_i]=gb_myValue;
			gb_i++;
		}    else
		{
			gb_buff[gb_i]='\0';
			break;
		}

	}
	while(!(USART1->SR & USART_SR_RXNE));
}
void GB_float_value1(float gb_value)
{
	char gb_float_buff[10];
	sprintf(gb_float_buff,"%f",gb_value);
	GB_printString1(gb_float_buff);

}


//************************STM32- UART2 functions*******************//
//PA3- RX, PA2-TX
void GB_uart_pin_conf2()
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

void GB_uart_boud_rate2()
{
	  //uint16_t uartdiv = SystemCoreClock / 16*9600;
	//usartdiv = 0d208.33
	//DIV_Fraction = 0.33*16 = 0d5.28 = 0d5(Nearest whole number) = 0x5
	//DIV_Mantissa = 0d208 = 0xd0
	//USART2->BRR =  ( ( uartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos );
	//USART2->BRR =  ( ( uartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) ;
     USART2->BRR = 0x116;
}
void GB_uart_init2()
{

	GB_uart_pin_conf2();
	//Alternate pin register
	/* no remap rx tx pins, for UART at pa2(tx) and pa3(rx)*/
    USART2->CR1 = 0x0000;
	//enable UART and configure stop byte
	USART2->CR1 |= USART_CR1_UE;
	USART2->CR1 &= ~USART_CR1_M;

	 //enable RXNE interrupts on USART SIDE
	USART2->CR1 |= USART_CR1_RXNEIE;

	//enable transmitter
	USART2->CR1 |= USART_CR1_TE;
	//enable receiver
	USART2->CR1 |= USART_CR1_RE;


    GB_uart_boud_rate2();


}
void GB_UART_TxChar2(uint32_t gb_byte)
{
	while(!(USART2->SR & USART_SR_TXE));
	USART2->DR = gb_byte;

}

void GB_printString2(const char *gb_myString)      /////to print any string
{
	while (*gb_myString)
	GB_UART_TxChar2(*gb_myString++);
	while(!(USART2->SR & USART_SR_TC));

}
void GB_bit2(uint32_t gb_val)
{
	int8_t gb_ptr;
	for(gb_ptr=31;gb_ptr>=0;gb_ptr--)
	{
		if ((gb_val & (1<<gb_ptr))==0)
		{
			GB_UART_TxChar2('0');
		}
		else
		{
			GB_UART_TxChar2('1');
		}
	}
}
void GB_decimel2(uint32_t gb_val)
{
	unsigned char gb_buf[5];
	int8_t gb_ptr;
	for(gb_ptr=0;gb_ptr<5;++gb_ptr)
	{
		gb_buf[gb_ptr] = (gb_val % 10) + '0';
		gb_val /= 10;
	}
	for(gb_ptr=4;gb_ptr>0;--gb_ptr)
	{
		if (gb_buf[gb_ptr] != '0')
		break;
	}
	for(;gb_ptr>=0;--gb_ptr)
	{
		GB_UART_TxChar2(gb_buf[gb_ptr]);
	}
}
void GB_uart_newline2()
{
	GB_UART_TxChar2('\n');
}
void GB_uart_rec2(char *gb_buff)
{
	int gb_i=0;
	uint16_t gb_myValue;
	do
	{

		gb_myValue = GB_UART_RxChar2();
		if(gb_myValue!='\n')
		{
			gb_buff[gb_i]=gb_myValue;
			gb_i++;
		}    else
		{
			gb_buff[gb_i]='\0';
			break;
		}

	}
	while(!(USART2->SR & USART_SR_RXNE));
}


void GB_float_value2(float gb_value)
{
	char gb_float_buff[10];
	sprintf(gb_float_buff,"%f",gb_value);
	GB_printString2(gb_float_buff);

}


