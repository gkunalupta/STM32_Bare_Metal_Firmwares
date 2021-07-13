/*
 * i2c.c
 *
 *  Created on: May 31, 2021
 *      Author: kunal
 */


#include "stm32f1xx.h"
#include "i2c.h"
//PB10-SCL,PB11-SDA
void I2C2_pin_config()
{


	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;


	//PB10 SCL will be set as alternate function open drain
	GPIOB->CRH |= GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1;      //Output Mode
	GPIOB->CRH |= GPIO_CRH_CNF10_1 | GPIO_CRH_CNF10_0;         //Alternate Function

 //PB11 SDA will be set as alternate function open drain
	GPIOB->CRH |= GPIO_CRH_MODE11_0 | GPIO_CRH_MODE11_1;     //Output Mode
	GPIOB->CRH |= GPIO_CRH_CNF11_1 | GPIO_CRH_CNF11_0;         //Alternate Function

}
uint32_t read_reg =0;
void master_init()
{
	I2C2_pin_config();

	// configure i2c_cr2 register for peripheral input clock: FREQ[] bits //
I2C2->CR2 = 0x20; //we have to set FREQ according to the frequency of our core
                  //that is 32MHZ in our case 0x20 = 32

//configure i2c_clock control register i2c_ccr
I2C2->CCR = 0x140;

	//configure i2c_trise register(i2c_trise)
I2C2->TRISE = 0x21;
	//program the i2c_cr1 register to enable the peripheral

	I2C2->CR1 |= I2C_CR1_PE;

	if((I2C2->SR2 & I2C_SR2_BUSY) == 1)
		stop_generation();


}

//when system  generates start signal , interface automatically switches to master mode
//MSL bit will tell whether entered in master mode or not
//SB bit to be in while loop for successful transmission of start bit

void i2c_start_condition_w() //EV5
{
	//generate the start condition for starting the communication
		I2C2->CR1 |= I2C_CR1_START;
		while(!(I2C2->SR1 & I2C_SR1_SB));  // wait to get transfered to master mode
		read_reg = I2C2->SR1;
}
void i2c_start_condition_r() //EV5
{
	I2C2->CR1 |= I2C_CR1_ACK;
	//generate the start condition for starting the communication
		I2C2->CR1 |= I2C_CR1_START;
		while(!(I2C2->SR1 & I2C_SR1_SB));  // wait to get transfered to master mode
		read_reg = I2C2->SR1;
}
/*
 * In 7-bit addressing mode,
– To enter Transmitter mode, a master sends the slave address with LSB reset.
– To enter Receiver mode, a master sends the slave address with LSB set.
 * */
void i2c_address_send_w(uint8_t slave_address)//EV6
{


	I2C2->DR = slave_address;//Write slave address on I2C data bus
	while(!(I2C2->SR1 & I2C_SR1_TXE));
	while(!(I2C2->SR1 & I2C_SR1_ADDR));  // Wait till address byte is being transmitted: ADDR bit set on succesful transmission of address

	//Clearing the ADDR bit
	read_reg = I2C2->SR1;
	read_reg = I2C2->SR2;

	//while(!(I2C2->SR1 & I2C_SR1_TXE)); //(EV8_1)wait for data register to get empty on transmission of data
}
void databyte_send(uint8_t data)//EV8
{
	I2C2->DR = data; // send data byte on data register
	while(!(I2C2->SR1 & I2C_SR1_TXE));  //wait for data register to get empty on transmission of data
}


int i2c_address_send_r(uint8_t slave_address)//EV6
{

	//I2C2->CR1 |= I2C_CR1_ACK;
	I2C2->DR = slave_address;//Write slave address on I2C data bus
	//while(!(I2C2->SR1 & I2C_SR1_TXE));
	while(!(I2C2->SR1 & I2C_SR1_ADDR))

	{
		if((I2C2->SR1 & I2C_SR1_AF) == 1)
		{
			printf("error in sendind read slave address\n");
			return 0;
		}

	}

	// Wait till address byte is being transmitted: ADDR bit set on succesful transmission of address

	//Clearing the ADDR bit(EV6)
	read_reg = I2C2->SR1;
	read_reg = I2C2->SR2;
}
//for receiving more than 3 bytes
void datareive_string(char data[])
{
	//disable ack
	I2C2->CR1 |= I2C_CR1_ACK;

	while(!(I2C2->SR1 & I2C_SR1_BTF));

	I2C2->CR1 &= I2C_CR1_ACK;



}
//for receiving 2 bytes
void databyte_receive_ack(uint8_t data1, uint8_t data2)
{
	//uint8_t data;
	I2C2->CR1 |= I2C_CR1_POS;

	I2C2->CR1 &= ~I2C_CR1_ACK;

	//while(!(I2C2->SR1 & I2C_SR1_RXNE));
	while(!(I2C2->SR1 & I2C_SR1_BTF));
	I2C2->CR1 |= I2C_CR1_STOP;
	data1 = I2C2->DR;
	data2 = I2C2->DR;

	//if(!(I2C2->SR1 & I2C_SR1_BTF)); // check whether data byte transmission is done or not: BTF is set
	//read_reg = I2C2->SR1;

	//return data;
}

// for receiving 1 byte
uint8_t databyte_receive_nack()
{
	uint8_t data;

	I2C2->CR1 &= ~I2C_CR1_ACK; //clear ACK bit
	//generate stop
	I2C2->CR1 |= I2C_CR1_STOP;

	while(!(I2C2->SR1 & I2C_SR1_RXNE));
	data = I2C2->DR;

	//if(!(I2C2->SR1 & I2C_SR1_BTF)); // check whether data byte transmission is done or not: BTF is set
	//read_reg = I2C->SR1;

	return data;
}

void lastdata_send(uint8_t data)
{
//	while(!(I2C->SR1 & I2C_SR1_TXE));
	I2C2->DR = data; // send data byte on data register
	while(!(I2C2->SR1 & I2C_SR1_TXE));  //wait for data register to get empty on transmission of data: TXE is set

	while(!(I2C2->SR1 & I2C_SR1_BTF)); // check whether data byte transnission is done or not: BTF is set

	//clear TXE and BTF by generating the stop condition
	I2C2->CR1 |= I2C_CR1_STOP;
		//read_reg = I2C->SR1;
}

void stop_generation()
{
	I2C2->CR1 |= I2C_CR1_STOP;

}
