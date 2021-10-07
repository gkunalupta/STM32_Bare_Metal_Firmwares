/*
 * gb_ina219.c
 *
 *  Created on: 15-Sep-2021
 *      Author: kunal
 */


/*
 *About Current sense Amplifiers : 1) https://www.ti.com/amplifier-circuit/current-sense/overview.html
 *                                 2) https://components101.com/articles/basics-of-current-sense-amplifier-its-types-and-design-guide
 * Current Sensing Techniques: https://components101.com/article/comparison-between-shunt-hall-and-ic-based-current-sensing-designs
 *
     ****************************************************************************
 *  INA219 has 2 main registers CALIBRATION and CONFIGURATION Register.
	 * To get the CURRENT and POWER Reading FROM INA219 CALIBRATION Register Has to be configured.
	 * For Programming The CALIBRATION Register see section 8.5.1 of Datasheet.
	 *
	 * CONFIGURATION Register is used for Setting the INA219 Module,
	 *
	 *
	 * Calculation of CALIBRATION REGISTER
	 * Now Maximum Load Voltage: VBUS_MAX = 32V ( BRNG = 1 in CONF Register)
	 * Maximum Voltage Across Shunt Resistor Configured : VSHUNT_MAX = 0.32V  ( PGA bits in CONF are set to 1)
	 * Shunt Resistor Value: RSHUNT = 0.1 ( INA219 has On board Shunt Resistor)
	 *
	 * MAX_SHUNT_CURRENT = VSHUNT_MAX/RSHUNT = 0.32V/0.1V = 3.2A
	 *
	 * MAX_SHUNT_CURRENT = MAX_EXPECTED_CURRENT = 3.2A
	 *
	 * Now
	 * Current_LSB      = MAX_EXPECTED_CURRENT/32767
	 *                  = 3.2A/32767 = 0.000097A per bit
	 * Taking it approx = 0.0001A per bit
	 *
	 * Current LSB = 0.0001uA per bit
	 * POWER_LSB = 20 * Current_LSB = 0.002  (2mW per bit)
	 *
	 *
	 * CALIBRATION Register = (0.04096/(Current_LSB * RSHUNT))
	 *                          = 0.04096/(0.0001 * 0.1)
	 *                          = 0d4096 = 0x1000
	 * CALIBATION Register = 4096
	 *
	 *  IN CONFIGURATION Register:  ADC resolution For Shunt and Bus, BUS VOLTAGE, PGA and SET The OPERATING MODE
	 *
	 ***************************************************************************************
	 * Now INA219 has 16 bit registers, But for Communicating Via I2C with INA219
	 * Data bytes has to be sent in 8 bit values.
	 *
	 *
	 * When Writing the 16 bit value of INA219 Register, devide the 16 bit Data Bytes of INA219 Registers at 2 bytes, each of 8 bit.
	 * 1) 1st byte having hibyte data(15-8 bits)
	 * 2) 2nd byte having lobyte data(0-7 bits)
	 *
	 *
	 * When Reading the 16 bit value from INA219 Register, We will Read the High and Low bytes
	 *  1) Then left shift the High Bytes by 8.
	 *  2) OR Operation on Left shifted MSB bits and LSB 8bits.
	 *
 */
#include "stm32f1xx.h"
#include "GB_uart.h"
#include "gb_ina219.h"
#include "gb_i2c.h"




float CR_LSB =0;
float PW_LSB =0;

/*
 * INA219 Initialise Function , for Setting  Configuration and Calibration register
 * as mentioned in above calculations
 */
void ina219_init()
{
	gb_i2c_master_init();



	CR_LSB = 0.0001;  // Current_LSB value is used to calculate Current after reading Current Register (0x04)
	PW_LSB = 0.002;   // Power_LSB value is used to calibrate Power after reading Power Register (0x03)


	uint16_t ina219_calvalue = 4096;   // CALIBRATION Register = 4096 from Calculation above
	uint8_t ina219_cal_temp;
	ina219_cal_temp = (uint16_t)ina219_calvalue;  // 1st byte having hibyte data(15-8 bits)
	ina219_calvalue >>= 8; // 2nd byte having lobyte data(0-7 bits)

	uint16_t ina219_confvalue = INA219_CONFIG_BVOLTAGERANGE_32V |
	INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
	INA219_CONFIG_SADCRES_12BIT_1S_532US |
	INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
	uint8_t ina219_conf_temp;
	ina219_conf_temp = (uint8_t)ina219_confvalue; // 1st byte having hibyte data(15-8 bits)
	ina219_confvalue >>= 8;  // 2nd byte having lobyte data(0-7 bits)


	gb_i2c_start_condition_w(); // Start Condition For Writing
	gb_i2c_address_send_w(ina219_WA); // INA219 I2C address is sent with Write bit
	gb_i2c_master_send_byte(ina219_CAL); // Sent Address of Calibration Register
	gb_i2c_master_send_byte((uint8_t)ina219_calvalue); // Calibration High Byte is sent
	gb_i2c_master_send_byte(ina219_cal_temp); //Calibration Low Byte is sent
	gb_i2c_master_stop_generation(); // STOP Condition is generated

	gb_i2c_start_condition_w();   // Start Condition For Writing
	gb_i2c_address_send_w(ina219_WA);   // INA219 I2C address is sent with Write bit
	gb_i2c_master_send_byte(ina219_CONF);   // Sent Address of Configuration Register
	gb_i2c_master_send_byte((uint8_t)ina219_confvalue);  // CONFIGURATION High Byte is sent
	gb_i2c_master_send_byte(ina219_conf_temp);  // CONFIGURATION LOW Byte is sent
	gb_i2c_master_stop_generation(); // STOP Condition is generated

}

/*
 * Function for reading the Bus Voltage Value from Bus Voltage Register(0x02) of INA219
 * -->  First sent the Address of Bus Voltage Register and then Read the 16 bits from that register
 * -->Bus Voltage Readings are given from [15:3] bytes, as [1:0] bits are Conversion Ready bit and Math Overflow Flag bit and [2] bit is reserved
 * --> so we will left shift the 16 bit value of 0x02 register by 3 bytes
 * --> And then Multiply the raw Bus Voltage Reading[15:3] by 4.
 */
void ina219_busvoltage()
{
	uint16_t ret; // Variable for Storing the  16 bit value
    uint8_t high_byte = 0;
    uint8_t low_byte = 0;

	gb_i2c_start_condition_w();  // Start Condition For Writing
	gb_i2c_address_send_w(ina219_WA);   // INA219 I2C address is sent with Write bit
	gb_i2c_master_send_byte(ina219_BV); // Sent Address of Bus Voltage Register
	gb_i2c_master_stop_generation();  // STOP Condition is generated


	gb_i2c_start_condition_r(); // Start Condition For Reading
	gb_i2c_address_send_r(ina219_RA); // INA219 I2C address is sent with Read bit
	gb_i2c_master_receive_2_bytes(&high_byte,&low_byte);  // Reading High and Low Bytes
	gb_i2c_master_stop_generation();



	ret = high_byte;
	ret <<= 8; // left shift the High Bytes by 8.
	ret |= low_byte; // OR Operation on Left shifted MSB bits and LSB 8bits to get the Final 16 bit Value

	uint16_t temp;
	float BV;
	temp = ret;
	temp >>= 3;  // Shift to the right 3 to drop CNVR and OVF
	BV =  temp * 0.004; //and multiply by LSB

	GB_float_value2(BV); // Print The Float value

}

/*
 * Function for reading the Shunt Voltage Value from Shunt Voltage Register(0x01) of INA219
 * -->First sent the Address of Shunt Voltage Register and then Read the 16 bits from that register
 * -->Shunt Voltage Readings are Read and Obtained 16 bit value is multiplied by 0.01
 */
void ina219_shuntvoltage()
{
	uint16_t ret;  // Variable for Storing the  16 bit value
	uint8_t high_byte = 0;
	uint8_t low_byte = 0;
	gb_i2c_start_condition_w(); // Start Condition For Writing
	gb_i2c_address_send_w(ina219_WA);// INA219 I2C address is sent with Write bit
	gb_i2c_master_send_byte(ina219_SV);  // Sent Address of Shunt Voltage Register
	gb_i2c_master_stop_generation();  // STOP Condition is generated


	gb_i2c_start_condition_r();  // Start Condition For Reading
	gb_i2c_address_send_r(ina219_RA);  // INA219 I2C address is sent with Read bit
	gb_i2c_master_receive_2_bytes(&high_byte,&low_byte);  // Reading High and Low Bytes
	gb_i2c_master_stop_generation(); // STOP Condition is generated
	ret = high_byte;
	ret <<= 8; // left shift the High Bytes by 8.
	ret |= low_byte; // OR Operation on Left shifted MSB bits and LSB 8bits to get the Final 16 bit Value

	float SV;
	SV =  ret * 0.01;

	GB_float_value2(SV);


}
/*
 * Function for reading the Shunt Current Value from Shunt Current Register(0x04) of INA219
 * -->First sent the Address of Shunt Current  Register and then Read the 16 bits from that register
 * -->Shunt Current Readings are Read and Obtained 16 bit value is multiplied by Current_LSB(0.0001) and then Multiplied to 1000 for making it convert to mA
 */
void ina219_shuntcurrent()
{
	uint16_t ret;  // Variable for Storing the  16 bit value
	uint8_t high_byte = 0;
	uint8_t low_byte = 0;
	gb_i2c_start_condition_w();  // Start Condition For Writing
	gb_i2c_address_send_w(ina219_WA);  // INA219 I2C address is sent with Write bit
	gb_i2c_master_send_byte(ina219_CR);  // Sent Address of Shunt Current Register
	gb_i2c_master_stop_generation();    // STOP Condition is generated


	gb_i2c_start_condition_r();    // Start Condition For Reading
	gb_i2c_address_send_r(ina219_RA);  // INA219 I2C address is sent with Read bit
	gb_i2c_master_receive_2_bytes(&high_byte,&low_byte);  // Reading High and Low Bytes
	gb_i2c_master_stop_generation(); // STOP Condition is generated
	ret = high_byte;
	ret <<= 8; // left shift the High Bytes by 8.
	ret |= low_byte; // OR Operation on Left shifted MSB bits and LSB 8bits to get the Final 16 bit Value

	float CR;
	CR =  ret * CR_LSB * 1000;

	GB_float_value2(CR);


}
/*
 * Function for reading the Power Consumed by Load  Value from Power Register(0x03) of INA219
 * -->First sent the Address of Power Register and then Read the 16 bits from that register
 * -->Power Readings are Read and Obtained 16 bit value is multiplied by Power_LSB(0.002) and then Multiplied to 1000 for making it convert to mW
 */
void ina219_Loadpower()
{
	uint16_t ret;  // Variable for Storing the  16 bit value
	uint8_t high_byte = 0;
	uint8_t low_byte = 0;
	gb_i2c_start_condition_w();  // Start Condition For Writing
	gb_i2c_address_send_w(ina219_WA);  // INA219 I2C address is sent with Write bit
	gb_i2c_master_send_byte(ina219_PW); // Sent Address of Power Register
	gb_i2c_master_stop_generation(); // STOP Condition is generated


	gb_i2c_start_condition_r();  // Start Condition For Reading
	gb_i2c_address_send_r(ina219_RA); // INA219 I2C address is sent with Read bit
	gb_i2c_master_receive_2_bytes(&high_byte,&low_byte);  // Reading High and Low Bytes
	gb_i2c_master_stop_generation(); // STOP Condition is generated
	ret = high_byte;
	ret <<= 8; // left shift the High Bytes by 8.
	ret |= low_byte; // OR Operation on Left shifted MSB bits and LSB 8bits to get the Final 16 bit Value

	float PW;
	PW =  ret * PW_LSB * 1000;

	GB_float_value2(PW);


}
