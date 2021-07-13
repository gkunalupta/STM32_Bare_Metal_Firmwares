/*
 * spi.c
 *
 *  Created on: 27-Mar-2021
 *      Author: kunal
 */


#include "stm32f1xx.h"
#include "spi.h"



void spi_pin_conf()
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	//PA4- CE,PA5-SCK,PA7-MOSI,PA6-MISO:

	//PA5 SCK will be set as alternate function output pushpull
	GPIOA->CRL |= GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1;      //Output Mode
	GPIOA->CRL |= GPIO_CRL_CNF5_1;         //Alternate Function
	GPIOA->CRL &=  ~(GPIO_CRL_CNF5_0);
 //PA7 MOSI will be set as alternate function output pushpull
	GPIOA->CRL |= GPIO_CRL_MODE7_0 | GPIO_CRL_MODE7_1;     //Output Mode
	GPIOA->CRL |= GPIO_CRL_CNF7_1;         //Alternate Function
	GPIOA->CRL &=  ~(GPIO_CRL_CNF7_0);
//PA4 CE will be set as General Purpose Output Mode
	GPIOA->CRL |= GPIO_CRL_MODE4_0 | GPIO_CRL_MODE4_1;     //Output Mode
	GPIOA->CRL &= ~GPIO_CRL_CNF4_1;         //General Purpose
	GPIOA->CRL &=  ~(GPIO_CRL_CNF4_0);  //GPIOA->CRL &=  ~(GPIO_CRL_CNF4_0);
//PA6 MISO will be set as Floating Input Mode
	GPIOA->CRL &= ~(GPIO_CRL_MODE6_0 | GPIO_CRL_MODE6_1);      //Input Mode
	GPIOA->CRL &= ~GPIO_CRL_CNF6_1;        //Floating Input
	GPIOA->CRL |=  (GPIO_CRL_CNF6_0);


}

void spi_init()
{

	spi_pin_conf();

	//BAUD RATE = FCK/32=1MHZ
	SPI1->CR1 &= ~( SPI_CR1_BR_0| SPI_CR1_BR_1);
	SPI1->CR1 |= SPI_CR1_BR_2;

	//SLAVE SELECT PIN SETING
	SPI1->CR1 &= ~(SPI_CR1_SSM); //SSM bit = 0
	SPI1->CR2 |= (SPI_CR2_SSOE);//SSOE bit = 1

	//MASTER SELECTION
	SPI1->CR1 |= SPI_CR1_MSTR;

	SPI1->CR1 |= SPI_CR1_SPE;

}

void spi_mast_tran_byte(uint8_t data)
{
	//GPIOA->ODR &= ~GPIO_ODR_ODR4;
	SPI1->DR = data;   //on writing data to data register, txe bit is cleared
	while(!(SPI1->SR & SPI_SR_TXE));    //when txe bit is set means data is shifted out from "out" pin
	while((SPI1->SR & SPI_SR_BSY));
	//GPIOA->ODR |= GPIO_ODR_ODR4;
}

uint8_t spi_mast_recv_byte()
{
	uint8_t recv_byte = 0;
	//GPIOA->ODR &= ~GPIO_ODR_ODR4;
	SPI1->DR = 0xff;
	while((SPI1->SR & SPI_SR_BSY));
	while((SPI1->SR & SPI_SR_RXNE))
	recv_byte = SPI1->DR;
	return recv_byte;

}
uint8_t spi_mast_exchange_byte(uint8_t data)
{
	GPIOA->ODR &= ~GPIO_ODR_ODR4;
	SPI1->DR = data;
	while(!(SPI1->SR & SPI_SR_TXE));

	while(!(SPI1->SR & SPI_SR_RXNE));
	return SPI1->DR;
	 GPIOA->ODR |= GPIO_ODR_ODR4;

}

