/*
 * gb_ssd1306.c
 *
 *  Created on: Oct 5, 2021
 *      Author: kunal
 */


#include "stm32f1xx.h"
#include "GB_uart.h"
#include "gb_ssd1306.h"
#include "gb_i2c.h"

static uint8_t GB_SSD1306_Buffer[GB_SSD1306_WIDTH * GB_SSD1306_HEIGHT / 8]; // buffer of 512 bytes

static const unsigned char SSD1306_font5x8[][GB_SSD1306_DFT_FONT_SIZE]=
{
    {0x00, 0x00, 0x00, 0x00, 0x00},   // space
    {0x00, 0x00, 0x2f, 0x00, 0x00},   // !
    {0x00, 0x07, 0x00, 0x07, 0x00},   // "
    {0x14, 0x7f, 0x14, 0x7f, 0x14},   // #
    {0x24, 0x2a, 0x7f, 0x2a, 0x12},   // $
    {0x23, 0x13, 0x08, 0x64, 0x62},   // %
    {0x36, 0x49, 0x55, 0x22, 0x50},   // &
    {0x00, 0x05, 0x03, 0x00, 0x00},   // '
    {0x00, 0x1c, 0x22, 0x41, 0x00},   // (
    {0x00, 0x41, 0x22, 0x1c, 0x00},   // )
    {0x14, 0x08, 0x3E, 0x08, 0x14},   // *
    {0x08, 0x08, 0x3E, 0x08, 0x08},   // +
    {0x00, 0x00, 0xA0, 0x60, 0x00},   // ,
    {0x08, 0x08, 0x08, 0x08, 0x08},   // -
    {0x00, 0x60, 0x60, 0x00, 0x00},   // .
    {0x20, 0x10, 0x08, 0x04, 0x02},   // /
    {0x3E, 0x51, 0x49, 0x45, 0x3E},   // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00},   // 1
    {0x42, 0x61, 0x51, 0x49, 0x46},   // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31},   // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10},   // 4
    {0x27, 0x45, 0x45, 0x45, 0x39},   // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30},   // 6
    {0x01, 0x71, 0x09, 0x05, 0x03},   // 7
    {0x36, 0x49, 0x49, 0x49, 0x36},   // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E},   // 9
    {0x00, 0x36, 0x36, 0x00, 0x00},   // :
    {0x00, 0x56, 0x36, 0x00, 0x00},   // ;
    {0x08, 0x14, 0x22, 0x41, 0x00},   // <
    {0x14, 0x14, 0x14, 0x14, 0x14},   // =
    {0x00, 0x41, 0x22, 0x14, 0x08},   // >
    {0x02, 0x01, 0x51, 0x09, 0x06},   // ?
    {0x32, 0x49, 0x59, 0x51, 0x3E},   // @
    {0x7C, 0x12, 0x11, 0x12, 0x7C},   // A
    {0x7F, 0x49, 0x49, 0x49, 0x36},   // B
    {0x3E, 0x41, 0x41, 0x41, 0x22},   // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C},   // D
    {0x7F, 0x49, 0x49, 0x49, 0x41},   // E
    {0x7F, 0x09, 0x09, 0x09, 0x01},   // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A},   // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F},   // H
    {0x00, 0x41, 0x7F, 0x41, 0x00},   // I
    {0x20, 0x40, 0x41, 0x3F, 0x01},   // J
    {0x7F, 0x08, 0x14, 0x22, 0x41},   // K
    {0x7F, 0x40, 0x40, 0x40, 0x40},   // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F},   // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F},   // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E},   // O
    {0x7F, 0x09, 0x09, 0x09, 0x06},   // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E},   // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46},   // R
    {0x46, 0x49, 0x49, 0x49, 0x31},   // S
    {0x01, 0x01, 0x7F, 0x01, 0x01},   // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F},   // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F},   // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F},   // W
    {0x63, 0x14, 0x08, 0x14, 0x63},   // X
    {0x07, 0x08, 0x70, 0x08, 0x07},   // Y
    {0x61, 0x51, 0x49, 0x45, 0x43},   // Z
    {0x00, 0x7F, 0x41, 0x41, 0x00},   // [
    {0x55, 0xAA, 0x55, 0xAA, 0x55},   // Backslash (Checker pattern)
    {0x00, 0x41, 0x41, 0x7F, 0x00},   // ]
    {0x04, 0x02, 0x01, 0x02, 0x04},   // ^
    {0x40, 0x40, 0x40, 0x40, 0x40},   // _
    {0x00, 0x03, 0x05, 0x00, 0x00},   // `
    {0x20, 0x54, 0x54, 0x54, 0x78},   // a
    {0x7F, 0x48, 0x44, 0x44, 0x38},   // b
    {0x38, 0x44, 0x44, 0x44, 0x20},   // c
    {0x38, 0x44, 0x44, 0x48, 0x7F},   // d
    {0x38, 0x54, 0x54, 0x54, 0x18},   // e
    {0x08, 0x7E, 0x09, 0x01, 0x02},   // f
    {0x18, 0xA4, 0xA4, 0xA4, 0x7C},   // g
    {0x7F, 0x08, 0x04, 0x04, 0x78},   // h
    {0x00, 0x44, 0x7D, 0x40, 0x00},   // i
    {0x40, 0x80, 0x84, 0x7D, 0x00},   // j
    {0x7F, 0x10, 0x28, 0x44, 0x00},   // k
    {0x00, 0x41, 0x7F, 0x40, 0x00},   // l
    {0x7C, 0x04, 0x18, 0x04, 0x78},   // m
    {0x7C, 0x08, 0x04, 0x04, 0x78},   // n
    {0x38, 0x44, 0x44, 0x44, 0x38},   // o
    {0xFC, 0x24, 0x24, 0x24, 0x18},   // p
    {0x18, 0x24, 0x24, 0x18, 0xFC},   // q
    {0x7C, 0x08, 0x04, 0x04, 0x08},   // r
    {0x48, 0x54, 0x54, 0x54, 0x20},   // s
    {0x04, 0x3F, 0x44, 0x40, 0x20},   // t
    {0x3C, 0x40, 0x40, 0x20, 0x7C},   // u
    {0x1C, 0x20, 0x40, 0x20, 0x1C},   // v
    {0x3C, 0x40, 0x30, 0x40, 0x3C},   // w
    {0x44, 0x28, 0x10, 0x28, 0x44},   // x
    {0x1C, 0xA0, 0xA0, 0xA0, 0x7C},   // y
    {0x44, 0x64, 0x54, 0x4C, 0x44},   // z
    {0x00, 0x10, 0x7C, 0x82, 0x00},   // {
    {0x00, 0x00, 0xFF, 0x00, 0x00},   // |
    {0x00, 0x82, 0x7C, 0x10, 0x00},   // }
    {0x00, 0x06, 0x09, 0x09, 0x06}    // ~ (Degrees)
};

typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} GB_SSD1306_t;

/* Private variable */
static GB_SSD1306_t GB_SSD1306;

void ssd1306_init()
{

	ssd1306_sendcommand(GB_SSD1306_DISPLAY_OFF); //0xAE
	ssd1306_sendcommand(GB_SSD1306_DISPLAY_CLOCK_DIV); //0xD5
	ssd1306_sendcommand(0x80);// A[7:4] = 1000, A[3:0] = 0000(clock divide ratio = 1)
	ssd1306_sendcommand(GB_SSD1306_MULTIPLEX_RATIO); //0xA8
	ssd1306_sendcommand(0x3F);

	ssd1306_sendcommand(GB_SSD1306_DISPLAY_OFFSET); //0xD3
	ssd1306_sendcommand(0x00); //Display start line starts from COM0 only, that is from ROW0
	ssd1306_sendcommand(0x40 | 0x00); //Display Start line: starting address of display RAM, by selecting a value from 0 to 63.
	ssd1306_sendcommand(GB_SSD1306_CHARGEUP);//0x8D We use internal charge pump
	ssd1306_sendcommand(0x14);

	ssd1306_sendcommand(GB_SSD1306_SET_MEMORY_ADDRESSING_COMMAND); //0x20
	ssd1306_sendcommand(GB_SSD1306_HORIZONTAL_ADDRESSING); //0x00
	ssd1306_sendcommand(GB_SSD1306_SEGMENT_REMAPPING_COM127_SEG0); //0xA0
	ssd1306_sendcommand(GB_SSD1306_COM_SCAN_DIRECTION_REMAPPED_MODE); //0xC8


	ssd1306_sendcommand(GB_SSD1306_COM_HARDWARE_CONFIG_COMMAND); //0xDA
	ssd1306_sendcommand(GB_SSD1306_COM_HARDWARE_CONFIG_ALTERNATE_DISABLE_REMAP);//0x02
	ssd1306_sendcommand(GB_SSD1306_CONTRAST); //0x81
	ssd1306_sendcommand(0xCF);

	ssd1306_sendcommand(GB_SSD1306_PRECHARGE);//0xD9
	ssd1306_sendcommand(0xF1);

	ssd1306_sendcommand(GB_SSD1306_VCOMH_SELECT_COMMAND);
	ssd1306_sendcommand(0x40); // to be checked
	ssd1306_sendcommand(GB_SSD1306_DISPLAY_RESUME);//0xA4
	ssd1306_sendcommand(GB_SSD1306_NORMAL_DISPLAY); //0xA6
	ssd1306_sendcommand(GB_SSD1306_DISPLAY_ON);

/*
	gb_i2c_start_condition_w(); // Start Condition For Writing
	gb_i2c_address_send_w(SSD1306_WA); // INA219 I2C address is sent with Write bit
	ssd1306_sendcommand(GB_SSD1306_CONTROL_BYTE_FOR_DATA); // Calibration High Byte is sent
	ssd1306_sendcommand('k'); //Calibration Low Byte is sent
	gb_i2c_master_stop_generation(); // STOP Condition is generated
	delay_ms(100);
	*/

	/* Set default values */
		GB_SSD1306.CurrentX = 0;
		GB_SSD1306.CurrentY = 0;

	/* Initialized OK */
	GB_SSD1306.Initialized = 1;

}

void ssd1306_sendcommand(uint8_t command)
{
	gb_i2c_start_condition_w(); // Start Condition For Writing
	gb_i2c_address_send_w(SSD1306_WA); // INA219 I2C address is sent with Write bit
	gb_i2c_master_send_byte(GB_SSD1306_CONTROL_BYTE_FOR_COMMAND); //0x00
	gb_i2c_master_send_byte(command);
	gb_i2c_master_stop_generation(); // STOP Condition is generated

}

void ssd1306_senddata(uint8_t data)
{
	gb_i2c_start_condition_w(); // Start Condition For Writing
	gb_i2c_address_send_w(SSD1306_WA); // INA219 I2C address is sent with Write bit
	gb_i2c_master_send_byte(GB_SSD1306_CONTROL_BYTE_FOR_DATA); //0x00
	gb_i2c_master_send_byte(data);
	gb_i2c_master_stop_generation(); // STOP Condition is generated

}
void ssd1306_update_data()
{
	ssd1306_sendcommand(GB_SSD1306_SET_COLUMN_ADDRESS_HV_ADDRESSING_MODE_COMMAND);
	ssd1306_sendcommand(GB_SSD1306_COLUMN_START_ADDRESS_HV_ADDRESSING_MODE);
	ssd1306_sendcommand(GB_SSD1306_COLUMN_END_ADDRESS_HV_ADDRESSING_MODE);

	ssd1306_sendcommand(GB_SSD1306_PAGE_ADDRESS_HV_ADDRESSING_MODE_COMMAND);
	ssd1306_sendcommand(GB_SSD1306_PAGE_START_ADDRESS_HV_ADDRESSING_MODE);
	ssd1306_sendcommand(GB_SSD1306_PAGE_END_ADDRESS_HV_ADDRESSING_MODE);

	  for (uint8_t packet = 0; packet < GB_SSD1306_HEIGHT; packet++) {
		  gb_i2c_start_condition_w(); // Start Condition For Writing
		  gb_i2c_address_send_w(SSD1306_WA); // INA219 I2C address is sent with Write bit
		  gb_i2c_master_send_byte(GB_SSD1306_CONTROL_BYTE_FOR_DATA);
	        for (uint8_t packet_byte = 0; packet_byte < 16; ++packet_byte) {
	        	gb_i2c_master_send_byte(GB_SSD1306_Buffer[packet*16+packet_byte]);
	        }
	  }
	        gb_i2c_master_stop_generation(); // STOP Condition is generated
}

/*
** This function is specific to the SSD_1306 OLED.
**
**  Arguments:
**      lineNo    -> Line Number  // from 0-7
**      cursorPos -> Cursor Position // from 0-127: columns, cursor position is column number
**
*/
void ssd1306_SetCursor( uint8_t lineNo, uint8_t cursorPos )
{
  /* Move the Cursor to specified position only if it is in range */
  if((lineNo <= GB_SSD1306_MAX_LINE) && (cursorPos < GB_SSD1306_MAX_SEG))
  {
    GB_SSD1306_LineNum   = lineNo;             // Save the specified line number
    GB_SSD1306_CursorPos = cursorPos;          // Save the specified cursor position
    ssd1306_sendcommand(0x21);              // cmd for the column start and end address
    ssd1306_sendcommand(cursorPos);         // column start addr
    ssd1306_sendcommand(GB_SSD1306_MAX_SEG-1); // column end addr
    ssd1306_sendcommand(0x22);              // cmd for the page start and end address
    ssd1306_sendcommand(lineNo);            // page start addr
    ssd1306_sendcommand(GB_SSD1306_MAX_LINE);  // page end addr
  }
}
/*
 * x is column[0-127]
 * y is row[0-63]
 */
void ssd1306_GotoXY( uint16_t x,uint16_t y)
{
	/* Set write pointers */
		GB_SSD1306.CurrentX = x;
		GB_SSD1306.CurrentY = y;
}
void ssd1306_GoToNextLine()
{
	/*
	  ** Increment the current line number.
	  ** roll it back to first line, if it exceeds the limit.
	  */
	  GB_SSD1306_LineNum++;
	  GB_SSD1306_LineNum = (GB_SSD1306_LineNum & GB_SSD1306_MAX_LINE);
	  ssd1306_SetCursor(GB_SSD1306_LineNum,0); /* Finally move it to next line */
}
/*
 *  Fills entire LCD with desired color
 */
void ssd1306_clear(GB_SSD1306_COLOR_t color)
{
	memset(GB_SSD1306_Buffer, (color == GB_SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(GB_SSD1306_Buffer));
}
/*
 * x is column[0-127] : x is equivalent to CurrentX
 * y is row[0-63] : y is equivalent to CurrentY
 */
void ssd1306_draw_pixel(uint16_t x, uint16_t y, GB_SSD1306_COLOR_t color)
{
	if (
			x >= GB_SSD1306_WIDTH ||
			y >= GB_SSD1306_HEIGHT
		) {
			/* Error */
			return;
		}

		/* Set color */
		if (color == GB_SSD1306_COLOR_WHITE) {
			GB_SSD1306_Buffer[x + (y / 8) * GB_SSD1306_WIDTH] |= 1 << (y % 8);
		} else {
			GB_SSD1306_Buffer[x + (y / 8) * GB_SSD1306_WIDTH] &= ~(1 << (y % 8));
		}
}

void ssd1306_toggle_invert()
{
	uint16_t i;
	/* Do memory toggle */
		for (i = 0; i < sizeof(GB_SSD1306_Buffer); i++) {
			GB_SSD1306_Buffer[i] = ~GB_SSD1306_Buffer[i];
		}
}


/*
 * Font size is 8*5:
 * 5 is no of columns: font width // x is width : CurrentX :0-127
 * 8 is no of rows: font height  //y is height : CurrentY : 0-63
 */
char ssd1306_print_char(char ch, GB_SSD1306_COLOR_t color)
{

	uint32_t i, b, j;

	for (i = 0; i < 5 ; i++)
	{
			b = SSD1306_font5x8[(ch - 32)][i]; //mapping the vales of bits for rinting the character ch

			if (color == GB_SSD1306_COLOR_WHITE)
			{
				GB_SSD1306_Buffer[GB_SSD1306.CurrentX + (GB_SSD1306.CurrentY / 8) * GB_SSD1306_WIDTH] |= b;
			}else
			{
				GB_SSD1306_Buffer[GB_SSD1306.CurrentX + (GB_SSD1306.CurrentY / 8) * GB_SSD1306_WIDTH] &= ~b;
			}

		  GB_SSD1306.CurrentX ++;
		}

	/* Return character written */
		return ch;




//	 char data_byte;
//	 uint8_t temp = 0; //row number
//
//	if(((GB_SSD1306_CursorPos + GB_SSD1306_FontSize) >= GB_SSD1306_MAX_SEG) || (ch == '\n'))
//	{
//		ssd1306_GoToNextLine();
//	}
//
//	 if( ch != '\n' )
//	  {
//
//	    /*
//	    ** In our font array (SSD1306_font), space starts in 0th index.
//	    ** But in ASCII table, Space starts from 32 (0x20).
//	    ** So we need to match the ASCII table with our font table.
//	    ** We can subtract 32 (0x20) in order to match with our font table.
//	    */
//	    ch -= 0x20;  //or c -= ' ';
//	    do
//	    {
//	      data_byte= SSD1306_font5x8[ch][temp]; //ch is column no
//		                                    // Get the data to be displayed from LookUptable
//	      GB_SSD1306_Buffer[temp] = data_byte;
//
//	      GB_SSD1306_CursorPos++;
//
//	      temp++;
//
//	    } while ( temp < GB_SSD1306_FontSize);
//	    //ssd1306_senddata(0x00);         //Display the data
////	 GB_SSD1306_CursorPos++;
//	  }

}

char ssd1306_print_string(char* str, GB_SSD1306_COLOR_t color)
{
	while (*str)
	{
		ssd1306_print_char(*str++, (GB_SSD1306_COLOR_t) color);
	}
	/* Everything OK, zero should be returned */
		return *str;
}

void ssd1306_print_binary(uint32_t gb_val, GB_SSD1306_COLOR_t color)
{
	int8_t gb_ptr;
	for(gb_ptr=31;gb_ptr>=0;gb_ptr--)
	{
		if ((gb_val & (1<<gb_ptr))==0)
		{
			ssd1306_print_char('0',  (GB_SSD1306_COLOR_t) color);
		}
		else
		{
			ssd1306_print_char('1',  (GB_SSD1306_COLOR_t) color);
		}
	}
}
void ssd1306_print_decimel(uint32_t gb_val, GB_SSD1306_COLOR_t color)
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
		ssd1306_print_char(gb_buf[gb_ptr], (GB_SSD1306_COLOR_t) color);
	}
}

void ssd1306_float(float gb_value, GB_SSD1306_COLOR_t color)
{
	char gb_float_buff[10];
	sprintf(gb_float_buff,"%2f",gb_value);
	ssd1306_print_string(gb_float_buff,(GB_SSD1306_COLOR_t) color );

}
