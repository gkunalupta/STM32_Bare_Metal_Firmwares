/*
 * i2c.c
 *
 *  Created on: May 31, 2021
 *      Author: kunal
 */


#include <gb_i2c.h>
#include "stm32f1xx.h"
#include "stdio.h"
//PB10-SCL,PB11-SDA
void gb_i2c2_pin_config()
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
uint32_t gb_read_reg =0;
void gb_i2c2_master_init()
{
	gb_i2c2_pin_config();

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
		gb_i2c2_master_stop_generation();


}

//when system  generates start signal , interface automatically switches to master mode
//MSL bit will tell whether entered in master mode or not
//SB bit to be in while loop for successful transmission of start bit

void gb_i2c2_start_condition_w() //EV5
{
	//generate the start condition for starting the communication
		I2C2->CR1 |= I2C_CR1_START;
		while(!(I2C2->SR1 & I2C_SR1_SB));  // wait to get transfered to master mode
		gb_read_reg = I2C2->SR1;
}
void gb_i2c2_start_condition_r() //EV5
{
	I2C2->CR1 |= I2C_CR1_ACK;
	//generate the start condition for starting the communication
		I2C2->CR1 |= I2C_CR1_START;
		while(!(I2C2->SR1 & I2C_SR1_SB));  // wait to get transfered to master mode
		gb_read_reg = I2C2->SR1;
}
/*
 * In 7-bit addressing mode,
– To enter Transmitter mode, a master sends the slave address with LSB reset.
– To enter Receiver mode, a master sends the slave address with LSB set.
 * */
void gb_i2c2_address_send_w(uint8_t gb_slave_address)//EV6
{


	I2C2->DR = gb_slave_address;//Write slave address on I2C data bus
	while(!(I2C2->SR1 & I2C_SR1_TXE));
	while(!(I2C2->SR1 & I2C_SR1_ADDR));  // Wait till address byte is being transmitted: ADDR bit set on succesful transmission of address

	//Clearing the ADDR bit
	gb_read_reg = I2C2->SR1;
	gb_read_reg = I2C2->SR2;

	//while(!(I2C2->SR1 & I2C_SR1_TXE)); //(EV8_1)wait for data register to get empty on transmission of data
}
void gb_i2c2_master_send_byte(uint8_t gb_data)//EV8
{
	I2C2->DR = gb_data; // send data byte on data register
	while(!(I2C2->SR1 & I2C_SR1_TXE));  //wait for data register to get empty on transmission of data
}


int gb_i2c2_address_send_r(uint8_t gb_slave_address)//EV6
{

	//I2C2->CR1 |= I2C_CR1_ACK;
	I2C2->DR = gb_slave_address;//Write slave address on I2C data bus
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
	gb_read_reg = I2C2->SR1;
	gb_read_reg = I2C2->SR2;
}
//for receiving more than 3 bytes
//void datareive_string(char data[])
//{
//	//disable ack
//	I2C2->CR1 |= I2C_CR1_ACK;
//
//	while(!(I2C2->SR1 & I2C_SR1_BTF));
//
//	I2C2->CR1 &= I2C_CR1_ACK;
//
//
//
//}
//for receiving 2 bytes
void gb_i2c2_master_receive_2_bytes(uint8_t *gb_data1, uint8_t *gb_data2)
{
	//uint8_t data;
	I2C2->CR1 |= I2C_CR1_POS;

	I2C2->CR1 &= ~I2C_CR1_ACK;

	//while(!(I2C2->SR1 & I2C_SR1_RXNE));
	while(!(I2C2->SR1 & I2C_SR1_BTF));
	I2C2->CR1 |= I2C_CR1_STOP;
	*gb_data1 = I2C2->DR;
	*gb_data2 = I2C2->DR;

//	GB_decimel2(gb_data1);
//	 GB_printString2("\n");
	//if(!(I2C2->SR1 & I2C_SR1_BTF)); // check whether data byte transmission is done or not: BTF is set
	//read_reg = I2C2->SR1;

	//return data;
}

// for receiving 1 byte
uint8_t gb_i2c2_master_receive_byte()
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

void gb_i2c2_master_stop_generation()
{
	I2C2->CR1 |= I2C_CR1_STOP;

}

//PB8-SCL,PB9-SDA
//i2c1
void gb_i2c1_pin_config()
{


	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;


	//PB8 SCL will be set as alternate function open drain
	GPIOB->CRH |= GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1;      //Output Mode
	GPIOB->CRH |= GPIO_CRH_CNF8_1 | GPIO_CRH_CNF8_0;         //Alternate Function

 //PB9 SDA will be set as alternate function open drain
	GPIOB->CRH |= GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1;     //Output Mode
	GPIOB->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_CNF9_0;         //Alternate Function

	AFIO->MAPR |= AFIO_MAPR_I2C1_REMAP; // AFIO I2C1 REMAPPING

}
uint32_t gb_i2c1_read_reg =0;
void gb_i2c1_master_init()
{
	gb_i2c1_pin_config();

	// configure i2c_cr2 register for peripheral input clock: FREQ[] bits //
I2C1->CR2 = 0x20; //we have to set FREQ according to the frequency of our core
                  //that is 32MHZ in our case 0x20 = 32

//configure i2c_clock control register i2c_ccr
I2C1->CCR = 0x140;

	//configure i2c_trise register(i2c_trise)
I2C1->TRISE = 0x21;
	//program the i2c_cr1 register to enable the peripheral

	I2C1->CR1 |= I2C_CR1_PE;

	if((I2C1->SR2 & I2C_SR2_BUSY) == 1)
		gb_i2c1_master_stop_generation();


}

//when system  generates start signal , interface automatically switches to master mode
//MSL bit will tell whether entered in master mode or not
//SB bit to be in while loop for successful transmission of start bit

void gb_i2c1_start_condition_w() //EV5
{
	//generate the start condition for starting the communication
		I2C1->CR1 |= I2C_CR1_START;
		while(!(I2C1->SR1 & I2C_SR1_SB));  // wait to get transfered to master mode
		gb_i2c1_read_reg = I2C1->SR1;
}
void gb_i2c1_start_condition_r() //EV5
{
	I2C1->CR1 |= I2C_CR1_ACK;
	//generate the start condition for starting the communication
		I2C1->CR1 |= I2C_CR1_START;
		while(!(I2C1->SR1 & I2C_SR1_SB));  // wait to get transfered to master mode
		gb_i2c1_read_reg = I2C1->SR1;
}
/*
 * In 7-bit addressing mode,
– To enter Transmitter mode, a master sends the slave address with LSB reset.
– To enter Receiver mode, a master sends the slave address with LSB set.
 * */
void gb_i2c1_address_send_w(uint8_t gb_slave_address)//EV6
{


	I2C1->DR = gb_slave_address;//Write slave address on I2C data bus
	while(!(I2C1->SR1 & I2C_SR1_TXE));
	while(!(I2C1->SR1 & I2C_SR1_ADDR));  // Wait till address byte is being transmitted: ADDR bit set on succesful transmission of address

	//Clearing the ADDR bit
	gb_read_reg = I2C1->SR1;
	gb_read_reg = I2C1->SR2;

	//while(!(I2C2->SR1 & I2C_SR1_TXE)); //(EV8_1)wait for data register to get empty on transmission of data
}
void gb_i2c1_master_send_byte(uint8_t gb_data)//EV8
{
	I2C1->DR = gb_data; // send data byte on data register
	while(!(I2C1->SR1 & I2C_SR1_TXE));  //wait for data register to get empty on transmission of data
}


int gb_i2c1_address_send_r(uint8_t gb_slave_address)//EV6
{

	//I2C2->CR1 |= I2C_CR1_ACK;
	I2C1->DR = gb_slave_address;//Write slave address on I2C data bus
	//while(!(I2C2->SR1 & I2C_SR1_TXE));
	while(!(I2C1->SR1 & I2C_SR1_ADDR))

	{
		if((I2C1->SR1 & I2C_SR1_AF) == 1)
		{
			printf("error in sendind read slave address\n");
			return 0;
		}

	}

	// Wait till address byte is being transmitted: ADDR bit set on succesful transmission of address

	//Clearing the ADDR bit(EV6)
	gb_i2c1_read_reg = I2C1->SR1;
	gb_i2c1_read_reg = I2C1->SR2;
}
//for receiving more than 3 bytes
//void datareive_string(char data[])
//{
//	//disable ack
//	I2C2->CR1 |= I2C_CR1_ACK;
//
//	while(!(I2C2->SR1 & I2C_SR1_BTF));
//
//	I2C2->CR1 &= I2C_CR1_ACK;
//
//
//
//}
//for receiving 2 bytes
void gb_i2c1_master_receive_2_bytes(uint8_t *gb_data1, uint8_t *gb_data2)
{
	//uint8_t data;
	I2C1->CR1 |= I2C_CR1_POS;

	I2C1->CR1 &= ~I2C_CR1_ACK;

	//while(!(I2C2->SR1 & I2C_SR1_RXNE));
	while(!(I2C1->SR1 & I2C_SR1_BTF));
	I2C1->CR1 |= I2C_CR1_STOP;
	*gb_data1 = I2C1->DR;
	*gb_data2 = I2C1->DR;

}

// for receiving 1 byte
uint8_t gb_i2c1_master_receive_byte()
{
	uint8_t data;

	I2C1->CR1 &= ~I2C_CR1_ACK; //clear ACK bit
	//generate stop
	I2C1->CR1 |= I2C_CR1_STOP;

	while(!(I2C1->SR1 & I2C_SR1_RXNE));
	data = I2C1->DR;

	//if(!(I2C2->SR1 & I2C_SR1_BTF)); // check whether data byte transmission is done or not: BTF is set
	//read_reg = I2C->SR1;

	return data;
}

void gb_i2c1_master_stop_generation()
{
	I2C1->CR1 |= I2C_CR1_STOP;

}
