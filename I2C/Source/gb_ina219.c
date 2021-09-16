/*
 * gb_ina219.c
 *
 *  Created on: 15-Sep-2021
 *      Author: kunal
 */





#include "stm32f1xx.h"
#include "GB_uart.h"
#include "gb_ina219.h"
#include "gb_i2c.h"




float CR_LSB =0;
float PW_LSB =0;
void ina219_init()
{
	gb_i2c_master_init();


	CR_LSB = 0.0001;
	PW_LSB = 0.002;


	uint16_t ina219_calvalue = 4096;
	uint8_t ina219_cal_temp;
	ina219_cal_temp = (uint16_t)ina219_calvalue;
	ina219_calvalue >>= 8;

	uint16_t ina219_confvalue = INA219_CONFIG_BVOLTAGERANGE_32V |
	INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
	INA219_CONFIG_SADCRES_12BIT_1S_532US |
	INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
	uint8_t ina219_conf_temp;
	ina219_conf_temp = (uint8_t)ina219_confvalue;
	ina219_confvalue >>= 8;


	gb_i2c_start_condition_w();
	gb_i2c_address_send_w(ina219_WA);
	gb_i2c_master_send_byte(ina219_CAL);
	gb_i2c_master_send_byte((uint8_t)ina219_calvalue);
	gb_i2c_master_send_byte(ina219_cal_temp);
	gb_i2c_master_stop_generation();

	gb_i2c_start_condition_w();
	gb_i2c_address_send_w(ina219_WA);
	gb_i2c_master_send_byte(ina219_CONF);
	gb_i2c_master_send_byte((uint8_t)ina219_confvalue);
	gb_i2c_master_send_byte(ina219_conf_temp);
	gb_i2c_master_stop_generation();

}


void ina219_busvoltage()
{
	uint16_t ret;
uint8_t rec_data1 = 0;
uint8_t rec_data2 = 0;
	gb_i2c_start_condition_w();
	gb_i2c_address_send_w(ina219_WA);
	gb_i2c_master_send_byte(ina219_BV);
	gb_i2c_master_stop_generation();


	gb_i2c_start_condition_r();
	gb_i2c_address_send_r(ina219_RA);
	gb_i2c_master_receive_2_bytes(&rec_data1,&rec_data2);




	ret = rec_data1;
	ret <<= 8;
	ret |= rec_data2;
	//gb_i2c_master_stop_generation();


// 	GB_decimel2(rec_data1);
// 	GB_printString2("\n");
//
// 	GB_decimel2(rec_data2);
// 	GB_printString2("\n");
	uint16_t temp;
	float BV;
	temp = ret;
	temp >>= 3;
	BV =  temp * 0.004;

// 	GB_decimel0(temp);
// 	GB_printString0("\n");

	GB_float_value2(BV);




}


void ina219_shuntvoltage()
{
	uint16_t ret;
	uint8_t rec_data1, rec_data2;
		gb_i2c_start_condition_w();
		gb_i2c_address_send_w(ina219_WA);
		gb_i2c_master_send_byte(ina219_SV);
		gb_i2c_master_stop_generation();


		gb_i2c_start_condition_r();
		gb_i2c_address_send_r(ina219_RA);
		gb_i2c_master_receive_2_bytes(&rec_data1,&rec_data2);
		ret = rec_data1;
		ret <<= 8;
		ret |= rec_data2;
		gb_i2c_master_stop_generation();

	float SV;
	SV =  ret * 0.01;

	GB_float_value2(SV);


}

void ina219_shuntcurrent()
{
	uint16_t ret;
	uint8_t rec_data1, rec_data2;
		gb_i2c_start_condition_w();
		gb_i2c_address_send_w(ina219_WA);
		gb_i2c_master_send_byte(ina219_CR);
		gb_i2c_master_stop_generation();


		gb_i2c_start_condition_r();
		gb_i2c_address_send_r(ina219_RA);
		gb_i2c_master_receive_2_bytes(&rec_data1,&rec_data2);
		ret = rec_data1;
		ret <<= 8;
		ret |= rec_data2;
		gb_i2c_master_stop_generation();

	float CR;
	CR =  ret * CR_LSB * 1000;

	GB_float_value2(CR);


}

void ina219_Loadpower()
{
	uint16_t ret;
	uint8_t rec_data1, rec_data2;
		gb_i2c_start_condition_w();
		gb_i2c_address_send_w(ina219_WA);
		gb_i2c_master_send_byte(ina219_PW);
		gb_i2c_master_stop_generation();


		gb_i2c_start_condition_r();
		gb_i2c_address_send_r(ina219_RA);
		gb_i2c_master_receive_2_bytes(&rec_data1,&rec_data2);
		ret = rec_data1;
		ret <<= 8;
		ret |= rec_data2;
		gb_i2c_master_stop_generation();

	float PW;
	PW =  ret * PW_LSB * 1000;

	GB_float_value2(PW);


}
