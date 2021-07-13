/*
 * w25q128jv.c
 *
 *  Created on: 15-Apr-2021
 *      Author: kunal
 */


#include "spi.h"
#include "w25q128jv.h"


void W25_Reset (void)
{
  cs_reset();
spi_mast_tran_byte(reset1);
spi_mast_tran_byte(reset2);
  cs_set();
}
void WriteEnable_flash()
{
	cs_reset();
	spi_mast_tran_byte(WriteEnable);
	cs_set();

}
void W25_Read_Data(uint32_t addr, char block[], uint32_t sz)
{
	volatile uint8_t recv_char=0;
	uint8_t i=0;
  cs_reset();
  spi_mast_tran_byte(ReadData);
  spi_mast_tran_byte(((addr>>16) & (0xff)));
  spi_mast_tran_byte(((addr>>8) & (0xff)));
  spi_mast_tran_byte(addr & 0xff);
  while ((sz--)!= 0)                             //Buffer for reading required number of bytes
  	{

  	recv_char= spi_mast_recv_byte();
  		block[i++] = recv_char;
  	}  cs_set();
}
void W25_Write_Data(uint32_t addr, char block[], uint32_t sz)
{
	WriteEnable_flash();
  cs_reset();
  spi_mast_tran_byte(WriteData);
  spi_mast_tran_byte(((addr>>16) & (0xff)));
  spi_mast_tran_byte(((addr>>8) & (0xff)));
  spi_mast_tran_byte(addr & 0xff);
  for(uint8_t i = 0;i<sz;i++)
  {
   spi_mast_tran_byte(block[i]);
   }
  cs_set();
}
uint32_t W25_Read_ID(void)
{
  uint8_t dt[4];
  cs_reset();
  spi_mast_tran_byte(JEDECID);
  dt[0] = spi_mast_recv_byte();
  dt[1] = spi_mast_recv_byte();
  dt[2] = spi_mast_recv_byte();
  cs_set();
  return (dt[0] << 16) | (dt[1] << 8) | dt[2];
}
void W25_Ini(void)
{
//  HAL_Delay(100);
  W25_Reset();
 // HAL_Delay(100);
 //  unsigned int id = W25_Read_ID();
  // HAL_UART_Transmit(&huart2,(uint8_t*)"\r\n",2,0x1000);
  // sprintf(str1,"ID:0x%X\r\n",id);
  // HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);

}
void erase_sector4KB(uint32_t addr)
{
	WriteEnable_flash();
	cs_reset();
	spi_mast_tran_byte(SectErase4KB);
	spi_mast_tran_byte(((addr>>16) & (0xff)));
	spi_mast_tran_byte(((addr>>8) & (0xff)));
	spi_mast_tran_byte(addr & 0xff);
	cs_set();
}
void erase_sector32KB(uint32_t addr)
{
	WriteEnable_flash();
	cs_reset();
	spi_mast_tran_byte(SectErase32KB);
	spi_mast_tran_byte(((addr>>16) & (0xff)));
	spi_mast_tran_byte(((addr>>8) & (0xff)));
	spi_mast_tran_byte(addr & 0xff);
	cs_set();
}
void erase_sector64KB(uint32_t addr)
{
	WriteEnable_flash();
	cs_reset();
	spi_mast_tran_byte(SectErase64KB);
	spi_mast_tran_byte(((addr>>16) & (0xff)));
	spi_mast_tran_byte(((addr>>8) & (0xff)));
	spi_mast_tran_byte(addr & 0xff);
	cs_set();
}
void chip_erase()
{
	WriteEnable_flash();
	cs_reset();
	spi_mast_tran_byte(chiperase);
	cs_set();
}
void Uinque_ID(uint8_t uinque[])
{
	cs_reset();

}
void WriteSR(uint8_t SR_address, uint8_t SR_data)
{
	WriteEnable_flash();
	cs_reset();
	spi_mast_tran_byte(SR_address);
	spi_mast_tran_byte(SR_data);
	cs_set();

}
uint8_t ReadSR(uint8_t SR_address)
{
	uint8_t readsr = 0;
	cs_reset();
	spi_mast_tran_byte(SR_address);
	readsr = spi_mast_recv_byte();
	cs_set();
	return readsr;
}
void WaitForWriteEnd(void)
{
	uint8_t StatusRegist1[1] = {0};
	cs_reset();
	spi_mast_tran_byte(ReadSR1);
	spi_mast_recv_byte();
	do
  {
		spi_mast_tran_byte(Dummybyte);
		StatusRegist1[0] = spi_mast_recv_byte();
    }
  while ((StatusRegist1[0] & 0x01) == 0x01);
  cs_set();
}
