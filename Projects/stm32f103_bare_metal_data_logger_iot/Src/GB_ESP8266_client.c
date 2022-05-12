/*
 * GB_ESP8266_Client.c
 *
 *  Created on: Aug 1, 2021
 *      Author: kunal
 */
#include <stdint.h>
#include "gb_t_delay.h"
#include "GB_uart.h"
#include "GB_ESP8266_Basic.h"
//#include "GB_ESP8266_Basic.c"
#include "GB_ESP8266_client.h"

char gb_get_command[100];
/* USART Receiver buffer Ring buffer for UART1 port*/
	#define gb_RX_BUFFER_SIZE_UART2 255

	extern char gb_RX_DATA_UART2[gb_RX_BUFFER_SIZE_UART2];

	//*************ESP Client Functions***********************
//	void GB_esp8266_initialise_client()
//	{
//
//		GB_esp8266_getmode();
//		GB_esp8266_connectiontype();
//		GB_esp8266_setAP("JioFiber 2.4ghz","Mansi5481");
//		GB_esp8266_getAP();
//
//		GB_printString1("\nSETAP Command\n");
//		//esp8266_setAP("JioFiber 2.4ghz","Mansi5481");
//		// esp8266_setAP("KG","12345678");
//		//esp8266_disconnect();
//		//esp8266_listAP();
//		//esp8266_IPadress();
//		//esp8266_rst();
//	}

		//AT+CIPSTART
		void GB_esp8266_connectTCPserver(const char * gb_url,int gb_port)
		{
			char gb_buff[100];
			sprintf(gb_buff,"AT+CIPSTART=\"TCP\",\"%s\",%u\r\n",gb_url,gb_port);
			GB_send_command(gb_buff);
			int gb_rsp = GB_RFR_UART2("OK", 2000);
			if(gb_rsp==1) GB_printString1("\n Successfully connected to server \n");
			if(gb_rsp==0) GB_printString1("\n Already connected to server \n");
		}
		//FOR CALCULATING LENGTH OF GET COMMAND
		char * GB_esp8266_command(const char * gb_urlpath)
		{
			//char get_command[100];
			memset(gb_get_command, '\0',sizeof(gb_get_command));
			sprintf(gb_get_command,"GET %s\r\n",gb_urlpath);
			//printString0(GET);
			//printString0("\n");
			return (char *) gb_get_command;
		}
		//FOR CALCULATING LENGTH OF GET COMMAND
		const char * GB_esp8266_commandint(const char * gb_urlpath, int gb_val)
		{
			//char get_command[100];
			memset(gb_get_command, '\0',sizeof(gb_get_command));
			sprintf(gb_get_command,"GET %s=%u\r\n",gb_urlpath,gb_val);
			GB_printString1(gb_get_command);
			GB_printString1("\n");
			return (char * )gb_get_command;
		}
		const char * GB_esp8266_commandfloat(const char * gb_urlpath, float gb_val)
			{
				//char get_command[100];
				memset(gb_get_command, '\0',sizeof(gb_get_command));
				sprintf(gb_get_command,"GET %s=%.2f\r\n",gb_urlpath,gb_val);
				GB_printString1(gb_get_command);
				GB_printString1("\n");
				return (char * )gb_get_command;
			}
		//FOR CALCULATING LENGTH OF GET COMMAND
		const char * GB_esp8266_commandchar(const char * gb_urlpath, char * gb_val)
		{
			//char get_command[100];
			memset(gb_get_command, '\0',sizeof(gb_get_command));
			sprintf(gb_get_command,"GET %s=%s\r\n",gb_urlpath,gb_val);
			GB_printString1(gb_get_command);
			GB_printString1("\n");
			return (char * )gb_get_command;
		}
		//AT+CIPSEND
		void GB_esp8266_cipsendint(int  gb_val, const char *  gb_urlpath)
		{

			char  gb_send[15];
			int  gb_get_command_length = strlen(GB_esp8266_commandint( gb_urlpath, gb_val));
			//decimel0(strlen(esp8266_command(urlpath,senseval)));
			//printString0("\n");
			//printString0(esp8266_command(urlpath));
			//printString0("\n");
			sprintf( gb_send,"AT+CIPSEND=%u\r\n", gb_get_command_length);
			GB_send_command( gb_send);
			//printString0(send);
			//int  gb_rsp = GB_RFR_UART2(">",2000);
			//if (rsp==1) printString0(RX_DATA_UART1);
			GB_printString1( gb_RX_DATA_UART2);

		}

		void GB_esp8266_cipsendfloat(float  gb_val, const char *  gb_urlpath)
		{

			char  gb_send[15];
			int  gb_get_command_length = strlen(GB_esp8266_commandfloat( gb_urlpath,gb_val));
			//decimel0(strlen(esp8266_command(urlpath,senseval)));
			//printString0("\n");
			//printString0(esp8266_command(urlpath));
			//printString0("\n");
			sprintf(gb_send,"AT+CIPSEND=%u\r\n",gb_get_command_length);
			GB_send_command(gb_send);
			//printString0(send);
		//	int gb_rsp = GB_RFR_UART2(">",2000);
			//if (rsp==1) printString0(RX_DATA_UART1);
			GB_printString1(gb_RX_DATA_UART2);

		}
		//AT+CIPSEND
		void GB_esp8266_cipsendchar(char *  gb_val, const char *  gb_urlpath)
		{

			char  gb_send[15];
			int  gb_get_command_length = strlen(GB_esp8266_commandchar( gb_urlpath, gb_val));
			//decimel0(strlen(esp8266_command(urlpath,senseval)));
			//printString0("\n");
			//printString0(esp8266_command(urlpath));
			//printString0("\n");
			sprintf( gb_send,"AT+CIPSEND=%u\r\n", gb_get_command_length);
			GB_send_command( gb_send);
			//printString0(send);
			//int  gb_rsp = GB_RFR_UART2(">",2000);
			//if (rsp==1) printString0(RX_DATA_UART1);
			GB_printString1( gb_RX_DATA_UART2);

		}

		//GET COMMAND
		void GB_esp8266_tcpgetcommand(const char * gb_urlpath)    //when no field value has to be updated like path for json file
		{
			char gb_buff[100];
			memset(gb_buff, '\0',sizeof(gb_buff));
			sprintf(gb_buff,"GET %s\r\n",gb_urlpath);
			GB_printString1(gb_buff);
			GB_send_command(gb_buff);
			GB_RFR_UART2("CLOSE", 2000);
			GB_printString1("\n");
			delay_ms(100);
			GB_printString1(gb_RX_DATA_UART2);
		}//GET COMMAND
		void GB_esp8266_tcpgetcommandint(const char * gb_urlpath, int gb_val)   //when field has to be updated like updating the sendor value.
		{    char gb_buff[100];
			memset(gb_buff, '\0',sizeof(gb_buff));
			sprintf(gb_buff,"GET %s=%u\r\n",gb_urlpath,gb_val);
			GB_printString1(gb_buff);
			GB_send_command(gb_buff);
			GB_RFR_UART2("CLOSE", 2000);
			GB_printString1("\n");
			delay_ms(100);
			GB_printString1(gb_RX_DATA_UART2);
		}//GET COMMAND
		void GB_esp8266_tcpgetcommandfloat(const char * gb_urlpath, float gb_val)   //when field has to be updated like updating the sendor value.
				{    char gb_buff[100];
					memset(gb_buff, '\0',sizeof(gb_buff));
					sprintf(gb_buff,"GET %s=%.2f\r\n",gb_urlpath,gb_val);
					GB_printString1(gb_buff);
					GB_send_command(gb_buff);
					GB_RFR_UART2("CLOSE", 2000);
					GB_printString1("\n");
					delay_ms(100);
					GB_printString1(gb_RX_DATA_UART2);
				}

		void GB_esp8266_tcpgetcommandchar(const char * gb_urlpath, char * gb_val)   //when field has to be updated like updating the sendor value.
		{    char gb_buff[100];
			memset(gb_buff, '\0',sizeof(gb_buff));
			sprintf(gb_buff,"GET %s=%s\r\n",gb_urlpath,gb_val);
			GB_printString1(gb_buff);
			GB_send_command(gb_buff);
			GB_RFR_UART2("CLOSE", 2000);
			GB_printString1("\n");
			delay_ms(100);
			GB_printString1(gb_RX_DATA_UART2);
		}
