/*
 * gb_ssd1306.h
 *
 *  Created on: Oct 5, 2021
 *      Author: kunal
 */

#ifndef GB_SSD1306_H_
#define GB_SSD1306_H_


/*
 * I2C in SSD1306 consist of SLAVE Address bit SA0, I2C Data Signal SDA, I2C Clock signal SCL.
 *
 * SSD1306 has to recognize the slave address before transmitting or receiving any information by I2C.
 * The SSD1306 will respond to 7 bit Slave Address send my master + 1 bit of R/W operation to be performed.
 * 7 bit address of SSD1306 is as follows
 * b7 b6 b5 b4 b3 b2 b1   (b0 : Read/Write bit)
 * 0  1  1  1  1  0  SA0  (R/W : 1 Read/0 Write)  //0x7B( 011110(11): last 2 bits depend on SA0 and R/W value)
 *
 * now D/C pin of SSD1306 IC act as SA0 bit:
 * D/C pin LOW means SA0 is 0 so slave address is 0111100 //0x79 (0111100(1):last bit value depends on R/W operation)
 * D/C pin HIGH means SA0 is 1 so slave address is 0111101 //0x7A (0111110(1): last bit will depend on R/W operation)
 *
 * Control byte consist of Co and D/c bits followed by 6 zeros
 * Control Byte:
 * b7 b6  b5 b4 b3 b2 b1 b0
 * Co D/W 0  0  0  0  0  0  (0xC0: 11000000)
 *
 * Co bit 0: transmssion of following info will be data bytes only
 * Co bit 1:
 *
 * D/C bit 0:transmission of following data byte will be interrupted as command
 * D/C bit 1:transmission of following data byte will be written to GDDRAM(data printed on screen)
 */



/*******Fundamental Command Table **********/


/****Continuous Horizontal Scroll Setup***************/
//A[7:0],E[7:0]
#define GB_SSD1306_DUMMY_BYTE_LOW 0x00h
//F[7:0]
#define GB_SSD1306_DUMMY_BYTE_HIGH 0xFF
//Horizontall scroll by 1 byte only)
#define GB_SSD1306_HORIZONTAL_SCROLL_DIRECTION_MASK 0x27 //(0010011(1)

enum
{
	GB_SSD1306_RIGHT_HORIZONTAL_SCROLL =  0x26,
	GB_SSD1306_LEFT_HORIZONTAL_SCROLL = 0x27
};

//B[2:0]
#define GB_SSD1306_START_PAGE_ADDRESS_HORIZONTAL_SCROLL_MASK 0x07
enum
{
	GB_SSD1306_START_PAGE0_HORIZONTAL_SCROLL = 0x00,
	GB_SSD1306_START_PAGE1_HORIZONTAL_SCROLL = 0x01,
	GB_SSD1306_START_PAGE2_HORIZONTAL_SCROLL = 0x02,
	GB_SSD1306_START_PAGE3_HORIZONTAL_SCROLL = 0x03,
	GB_SSD1306_START_PAGE4_HORIZONTAL_SCROLL = 0x04,
	GB_SSD1306_START_PAGE5_HORIZONTAL_SCROLL = 0x05,
	GB_SSD1306_START_PAGE6_HORIZONTAL_SCROLL = 0x06,
	GB_SSD1306_START_PAGE7_HORIZONTAL_SCROLL = 0x07
};
//C[2:0]
#define GB_SSD1306_TIME_INTERVAL__HORIZONTAL_SCROLL_SCROLL_MASK 0x07
enum
{
	GB_SSD1306_TIME_INTERVAL_5_FRAMES_HORIZONTAL_SCROLL = 0x00,
	GB_SSD1306_TIME_INTERVAL_64_FRAMES_HORIZONTAL_SCROLL = 0x01,
	GB_SSD1306_TIME_INTERVAL_128_FRAMES_HORIZONTAL_SCROLL = 0x02,
	GB_SSD1306_TIME_INTERVAL_256_FRAMES_HORIZONTAL_SCROLL = 0x03,
	GB_SSD1306_TIME_INTERVAL_3_FRAMES_HORIZONTAL_SCROLL = 0x04,
	GB_SSD1306_TIME_INTERVAL_4_FRAMES_HORIZONTAL_SCROLL = 0x05,
	GB_SSD1306_TIME_INTERVAL_25_FRAMES_HORIZONTAL_SCROLL = 0x06,
	GB_SSD1306_TIME_INTERVAL_2_FRAMES_HORIZONTAL_SCROLL = 0x07,

};
//D[2:0]
#define GB_SSD1306_END_PAGE_ADDRESS_HORIZONTAL_SCROLL_MASK 0x07

enum
{
	GB_SSD1306_END_PAGE0_HORIZONTAL_SCROLL = 0x00,
	GB_SSD1306_END_PAGE1_HORIZONTAL_SCROLL = 0x01,
	GB_SSD1306_END_PAGE2_HORIZONTAL_SCROLL = 0x02,
	GB_SSD1306_END_PAGE3_HORIZONTAL_SCROLL = 0x03,
	GB_SSD1306_END_PAGE4_HORIZONTAL_SCROLL = 0x04,
	GB_SSD1306_END_PAGE5_HORIZONTAL_SCROLL = 0x05,
	GB_SSD1306_END_PAGE6_HORIZONTAL_SCROLL = 0x06,
	GB_SSD1306_END_PAGE7_HORIZONTAL_SCROLL = 0x07
};

/********************Continuous Vertical and Horizontal Scroll Setup****************/
#define GB_SSD1306_HORIZONTAL_VERTICAL_SCROLL_DIRECTION_MASK 0x2B //(001010(11) : x1,x0 bits value will decide
enum
{
	GB_SSD1306_VERTICAL_RIGHT_HORIZONTAL_SCROLL = 0x29, //X1X0=01
	GB_SSD1306_VERTICAL_LEFT_HORIZONTAL_SCROLL = 0x2A   //X1X0=10

};
//B[2:0]
#define GB_SSD1306_START_PAGE_ADDRESS_MASK 0x07
enum
{
	GB_SSD1306_START_PAGE0_VERTICAL_SCROLL = 0x00,
	GB_SSD1306_START_PAGE1_VERTICAL_SCROLL = 0x01,
	GB_SSD1306_START_PAGE2_VERTICAL_SCROLL = 0x02,
	GB_SSD1306_START_PAGE3_VERTICAL_SCROLL = 0x03,
	GB_SSD1306_START_PAGE4_VERTICAL_SCROLL = 0x04,
	GB_SSD1306_START_PAGE5_VERTICAL_SCROLL = 0x05,
	GB_SSD1306_START_PAGE6_VERTICAL_SCROLL = 0x06,
	GB_SSD1306_START_PAGE7_VERTICAL = 0x07
};

//C[2:0]
#define GB_SSD1306_TIME_INTERVAL_SCROLL_MASK 0x07
enum
{
	GB_SSD1306_TIME_INTERVAL_5_FRAMES_VERTICAL_SCROLL   = (0x00),
	GB_SSD1306_TIME_INTERVAL_64_FRAMES_VERTICAL_SCROLL  = (0x01),
	GB_SSD1306_TIME_INTERVAL_128_FRAMES_VERTICAL_SCROLL = (0x02),
	GB_SSD1306_TIME_INTERVAL_256_FRAMES_VERTICAL_SCROLL = (0x03),
	GB_SSD1306_TIME_INTERVAL_3_FRAMES_VERTICAL_SCROLL   = (0x04),
	GB_SSD1306_TIME_INTERVAL_4_FRAMES_VERTICAL_SCROLL   = (0x05),
	GB_SSD1306_TIME_INTERVAL_25_FRAMES_VERTICAL_SCROLL   = (0x06),
	GB_SSD1306_TIME_INTERVAL_2_FRAMES_VERTICAL_SCROLL   = (0x07),

};

//D[2:0]:Value of D[2:0] must be greater then B[2:0]
#define GB_SSD1306_END_PAGE_ADDRESS_VERTICAL_SCROLL_MASK 0x07
enum
{
	GB_SSD1306_END_PAGE0_VERTICAL_SCROLL = 0x00,
	GB_SSD1306_END_PAGE1_VERTICAL_SCROLL = 0x01,
	GB_SSD1306_END_PAGE2_VERTICAL_SCROLL = 0x02,
	GB_SSD1306_END_PAGE3_VERTICAL_SCROLL = 0x03,
	GB_SSD1306_END_PAGE4_VERTICAL_SCROLL = 0x04,
	GB_SSD1306_END_PAGE5_VERTICAL_SCROLL = 0x05,
	GB_SSD1306_END_PAGE6_VERTICAL_SCROLL = 0x06,
	GB_SSD1306_END_PAGE7_VERTICAL_SCROLL = 0x07
};

/*
 *E[5:0]: E[5:0]= 01h refer to offset =1 row ,E[5:0] =3Fh refer to offset =63 rows,
 *so add the row number the row munber (n), that is 0<n<64, with below Macro to make the corresponding
 *so vertical scroll offset
*/
#define GB_SSD1306_VERTICAL_SCROLL_OFFSET_MASk 0x00

//Stop scrolling that is configured by command 26h/27h/29h/2Ah.
#define GB_SSD1306_DEACTIVATE_SCROLL 0x2E
/*
 * Start scrolling that is configured by the scrolling setup commands :26h/27h/29h/2Ah with the following valid sequences:
 * Valid command sequence 1: 26h ;2Fh.
 * Valid command sequence 2: 27h ;2Fh.
 * Valid command sequence 3: 29h ;2Fh.
 * Valid command sequence 4: 2Ah ;2Fh.
*/
#define GB_SSD1306_ACTIVATE_SCROLL 0x2F


/**************************Scrolling Command Table***************************/
#define GB_SSD1306_VERTICAL_SCROLL_AREA_SETTING_COMMAND 0xA3

/****************Addressing Setting Command Table ************************************/

#define GB_SSD1306_SET_MEMORY_ADDRESSING_COMMAND 0x20

#define GB_SSD1306_SET_ADDRESSING_MODE_MASK 0x03
enum
{
	GB_SSD1306_HORIZONTAL_ADDRESSING = 0x00,
	GB_SSD1306_VERTICAL_ADDRESSING = 0x01,
	GB_SSD1306_PAGE_ADDRESSING = 0x02,
	GB_SSD1306_INVALID_ADDRESSING= 0x03,
};

// Below commands are only for horizontal or vertical addressing mode.
#define GB_SSD1306_SET_COLUMN_ADDRESS_HV_ADDRESSING_MODE_COMMAND 0x21
enum
{
	//A[6:0] : Column start address, range : 0-127d, (RESET=0d)
	GB_SSD1306_COLUMN_START_ADDRESS_HV_ADDRESSING_MODE = 0x00,
	//B[6:0]: Column end address, range : 0-127d,(RESET =127d)
	GB_SSD1306_COLUMN_END_ADDRESS_HV_ADDRESSING_MODE = 0x7F,
};



#define GB_SSD1306_PAGE_ADDRESS_HV_ADDRESSING_MODE_COMMAND 0x22
enum
{
	//A[2:0] : Page start Address, range : 0-7d, (RESET = 0d)
	GB_SSD1306_PAGE_START_ADDRESS_HV_ADDRESSING_MODE = 0x00,
	//B[2:0] : Page end Address, range : 0-7d, (RESET = 7d)
	GB_SSD1306_PAGE_END_ADDRESS_HV_ADDRESSING_MODE = 0x7D,
};
/*
 * In Horizontal_Vertical Addressing Mode
 * Follwoing sequence is used for starting RAM Pointer location
 * • Set the column start and end address of the target display location by command 21h
 * i2c_master_start();
 * i2c_master_write_address(SLA+W);
 * --> • Set the column start and end address of the target display location by command 21h.
 * i2c_master_send_byte(GB_SSD1306_SET_COLUMN_ADDRESS_HV_ADDRESSING_MODE_COMMAND);
 * i2c_master_send_byte(GB_SSD1306_COLUMN_START_ADDRESS_HV_ADDRESSING_MODE);
 * i2c_master_send_byte(GB_SSD1306_COLUMN_END_ADDRESS_HV_ADDRESSING_MODE);
 * --> • Set the page start and end address of the target display location by command 22h.
 * i2c_master_send_byte(GB_SSD1306_PAGE_ADDRESS_HV_ADDRESSING_MODE_COMMAND);
 * i2c_master_send_byte(GB_SSD1306_PAGE_START_ADDRESS_HV_ADDRESSING_MODE);
 * i2c_master_send_byte(GB_SSD1306_PAGE_END_ADDRESS_HV_ADDRESSING_MODE);
 *
 *
*/

//Below Commands are only for Page Addressing Mode

/*
 * so add the page number (n), that is 0<n<8, with below Macro to make the corresponding page as start
 * (PAGE0~PAGE7) for Page Addressing Mode using X[2:0].
 *
 */
#define GB_SSD1306_PAGE_START_ADDRESS_PAGE_ADDRESSING_MODE_MASK 0xB0
/*
 * so add the column number (n), that is 0<n<15, with below Macro to make the corresponding lower nibble of column start
 * address:X3 X2 X1 X0: [ 0-15]
 * 00-0F
 *
 */
#define GB_SSD1306_LOWER_COLUMN_START_NIBBLE_PAGE_ADDRESSING_MODE 0x00
/*
 * so add the column number (n), that is 0<n<15, with below Macro to make the corresponding higher nibble of column start
 * address:X3 X2 X1 X0: [ 0=15]
 * 10-1F
 *
 * E.G
 * lower column address is 03h
 * higher column address is 10h
 * lower column address's, lower nibble is LSB bytes of Column address : 3
 * higher column address's lower bibble is MSB bytes of Column address : 0
 * so combined starting column address is 03h
 * */
#define GB_SSD1306_HIGHER_COLUMN_START_ADDRESS_PAGE_ADDRESSING_MODE 0x10
/*
 * In page addressing mode
 * Follwoing sequence is used for starting RAM Pointer location
 * i2c_master_start();
 * i2c_naster_write_address(SLA+W);
 * --> Set the page start address of the target display location by command B0h to B7h.
 * i2c_master_send_databyte(GB_SSD1306_PAGE_START_ADDRESS_PAGE_ADDRESSING_MODE_MASK); // starting from page 0
 * -->Set the lower start column address of pointer by command 00h~0Fh.
 * i2c_master_send_databyte(GB_SSD1306_LOWER_COLUMN_START_NIBBLES_PAGE_ADDRESSING_MODE); // Lower nible column start 0
 * -->Set the upper start column address of pointer by command 10h~1Fh.
 * i2c_master_send_databyte(GB_SSD1306_HIGHER_COLUMN_START_ADDRESS_PAGE_ADDRESSING_MODE); //Higher nibble column start

*/

#define SSD1306_WA 0x78
#define SSD1306_RA 0x79

#define GB_SSD1306_CONTROL_BYTE_FOR_DATA 0x40 //((01)000000 : Co= 0, D/C pin = 1, followed by 6 zeros)
#define GB_SSD1306_CONTROL_BYTE_FOR_COMMAND 0x00
#define GB_SSD1306_DISPLAY_ON 0xAF
#define GB_SSD1306_DISPLAY_OFF 0xAE
#define GB_SSD1306_DISPLAY_CLOCK_DIV 0xD5
#define GB_SSD1306_MULTIPLEX_RATIO 0xA8
#define GB_SSD1306_DISPLAY_OFFSET 0xD3
#define GB_SSD1306_CHARGEUP 0x8D
#define GB_SSD1306_SEGMENT_REMAPPING_COM0_SEG0 0xA0
#define GB_SSD1306_SEGMENT_REMAPPING_COM127_SEG0 0xA1
#define GB_SSD1306_COM_SCAN_DIRECTION_NORMAL_MODE 0xC0
#define GB_SSD1306_COM_SCAN_DIRECTION_REMAPPED_MODE 0xC8
#define GB_SSD1306_COM_HARDWARE_CONFIG_COMMAND 0xDA


enum
{
	GB_SSD1306_COM_HARDWARE_CONFIG_SEQUEN_DISABLE_REMAP = 0x02,
	GB_SSD1306_COM_HARDWARE_CONFIG_SEQUEN_ENABLE_REMAP = 0x22,
	GB_SSD1306_COM_HARDWARE_CONFIG_ALTERNATE_DISABLE_REMAP = 0x12,
	GB_SSD1306_COM_HARDWARE_CONFIG_ALTERNATE_ENABLE_REMAP = 0x32,
};



#define GB_SSD1306_CONTRAST 0x81
#define GB_SSD1306_PRECHARGE 0xD9

#define GB_SSD1306_VCOMH_SELECT_COMMAND 0xDB
#define GB_SSD1306_VCOMH_0.65*VCC 0x00
#define GB_SSD1306_VCOMH_0.77*VCC 0x20
#define GB_SSD1306_VCOMH_0.83*VCC 0x30


#define GB_SSD1306_DISPLAY_RESUME 0xA4
#define GB_SSD1306_ENTIRE_DISPLAY_ON 0xA5
#define GB_SSD1306_NORMAL_DISPLAY 0xA6
#define GB_SSD1306_INVERSE_DISPLAY 0xA7

#define GB_SSD1306_MAX_LINE 7   //no of lines
#define GB_SSD1306_MAX_SEG 128  // no of rows
#define GB_SSD1306_DFT_FONT_SIZE 5
#define GB_SSD1306_WIDTH 128
#define GB_SSD1306_HEIGHT 64
/*
** Variable to store Line Number and Cursor Position.
*/
static uint8_t GB_SSD1306_LineNum   = 0;
static uint8_t GB_SSD1306_CursorPos = 0;
static uint8_t GB_SSD1306_FontSize  = GB_SSD1306_DFT_FONT_SIZE;

typedef enum {
	GB_SSD1306_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	GB_SSD1306_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} GB_SSD1306_COLOR_t;

void ssd1306_init();
void ssd1306_sendcommand(uint8_t command);
void ssd1306_update_data();
void ssd1306_senddata(uint8_t data);
void ssd1306_SetCursor( uint8_t lineNo, uint8_t cursorPos);
void ssd1306_GoToNextLine();
void ssd1306_clear(GB_SSD1306_COLOR_t color);
void ssd1306_draw_pixel(uint16_t x, uint16_t y, GB_SSD1306_COLOR_t color);
void ssd1306_toggle_invert();
char ssd1306_print_char(char ch,GB_SSD1306_COLOR_t color);
void ssd1306_GotoXY(uint16_t x,uint16_t y);
char ssd1306_print_string(char* str, GB_SSD1306_COLOR_t color);
void ssd1306_print_decimel(uint32_t gb_val, GB_SSD1306_COLOR_t color);
void ssd1306_print_binary(uint32_t gb_val, GB_SSD1306_COLOR_t color);
void ssd1306_float(float gb_value, GB_SSD1306_COLOR_t color);
void ssd1306_fill_byte(unsigned char data, GB_SSD1306_COLOR_t color);
void ssd1306_DrawLineV(uint8_t line_starting_point, uint8_t line_end_point, uint8_t column,uint8_t line_width);
void ssd1306_DrawLineH(uint8_t line_starting_point, uint8_t line_end_point, uint8_t row, uint8_t line_width);

#endif /* GB_SSD1306_H_ */
