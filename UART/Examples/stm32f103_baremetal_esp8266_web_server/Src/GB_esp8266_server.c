#include <stdint.h>
#include "t_delay.h"
#include "GB_uart.h"
#include "GB_ESP8266_Basic.h"
#include "GB_ESP8266_server.h"

	#define gb_RX_BUFFER_SIZE_UART2 500

	extern char gb_RX_DATA_UART2[gb_RX_BUFFER_SIZE_UART2];

	extern char gb_Rx_Buffer_UART2[gb_RX_BUFFER_SIZE_UART2];

	uint32_t gb_flag=0;
		char * gb_p;
		char * gb_q;
		int gb_ID=0;
		uint8_t gb_x=0;

//Function to wait for client to browse Ip address on Browser
//gb_flag is Parameter which
uint32_t GB_getstring_ESP8266FORSERVER(char* gb_IP)
{
	//GB_decimel0(gb_flag);

//	while (gb_flag==0)
	do 
	{
		GB_printString1(gb_RX_DATA_UART2);
		//Gets the updates data if client is connected
		GB_getstring_UART2();

		gb_p= strstr(gb_RX_DATA_UART2,"Host");  // when using client as web browser
		 //gb_p= strstr(gb_RX_DATA_UART2,"CONNECT"); // when using client as TCP from herculus or other esp8266 node
		 GB_printString1(gb_p);
		//}
		if(gb_p)    //IF Client is connected
		{
			gb_flag=1;
 			GB_printString1(gb_RX_DATA_UART2);
// 			
// 			GB_printString1(gb_p);
			GB_printString1("\n*******Client Connected*******\n");
			//gb_q = gb_RX_DATA_UART2 - gb_p;
			//GB_printString1(gb_q);
			gb_q = strstr(gb_RX_DATA_UART2,"+IPD");
			gb_q += strlen("+IPD");
			gb_channelid = *(gb_q+1);
// 			GB_printString1(gb_RX_DATA_UART2);
// 			GB_printString1("\n");
			GB_printString1("Channel ID with Client is:");
			GB_UART_TxChar1(gb_channelid);
			gb_flag=1;
			GB_printString1("\n");
// 			GB_decimel0(gb_flag);
// 			GB_printString1("\n");
			memset(gb_RX_DATA_UART2, '\0',gb_RX_BUFFER_SIZE_UART2);
			memset(gb_Rx_Buffer_UART2, '\0',gb_RX_BUFFER_SIZE_UART2);
			return 1;
		}


	}while (gb_flag==0);
		
	
	return 0;
}

char gb_webpagedata[500];
char gb_signMessage[] = "<!DOCTYPE html> <!-- kunal Gupta--> <html lang=\"en\"> <head>     <meta charset=\"UTF-8\">     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">     <meta name=\"author\" content=\"Kunal\">     <title>Gettobyte</title>     <style> 		body {background-color: rgb(207, 245, 191);} 		body {color: rgb(17, 14, 15);} 		p {color: rgb(9, 9, 12);} 	  </style> </head> <body> Gettobyte &nbsp;&nbsp;&nbsp;&nbsp;: Reinventing Tech with Bytes </body> </html>";
//function to find length of data to send
//Function to send AT+CIPSEND command
char gb_send[50];

void GB_esp8266_serverdatatoclientcipsend()
{
	GB_printString1("\n****Sending details of data to be send (AT+CIPSEND)********\n");
	//int get_command_length = strlen(webpage);
	int gb_get_command_length = strlen(gb_signMessage);
	sprintf(gb_send,"AT+CIPSEND=%i,%u\r\n",gb_channelid-48,gb_get_command_length+2);
	GB_printString1("Channel id is :");
	GB_UART_TxChar1(gb_channelid);
//	GB_printString1("\n");
//	GB_printString1(gb_send);
	//_delay_ms(10);
//	GB_printString1("\nK\n");
	GB_send_command(gb_send);
//	GB_printString1("\nG\n");
	//_delay_ms(10);
	//GB_send_command("AT+CIPSEND=1,460\r\n");
	//GB_decimel1(GB_RFR_UART2(">",2000));
	//GB_printString1("\nY\n");
	GB_RFR_UART2(">",2000);
	GB_printString1(gb_RX_DATA_UART2);
	//GB_printString1("\nN\n");
	
// 	int gb_rsp = GB_RFR_UART2(">",2000);
// 	GB_decimel0(gb_rsp);
// 	GB_printString1(gb_RX_DATA_UART2);
// 	if (gb_rsp==0) //printString1(RX_DATA_UART2);
// 	GB_printString1(gb_RX_DATA_UART2);
	
}
uint8_t webdata_debug = 0;
void GB_esp8266_webpagedata()
{
	webdata_debug = 1;
	GB_printString1("\n************Sending Data to Client************\n");
	//send_command(webpage);
	//_delay_ms(10);
	
	GB_printString1("\nZ\n");
	GB_send_command(gb_signMessage);
	GB_printString1("\nP\n");
	GB_decimel1(GB_RFR_UART2("SEND OK",2000));
	GB_printString1("\nM\n");
	GB_printString1(gb_RX_DATA_UART2);
	GB_printString1("\nO\n");
	
	//GB_printString1(gb_RX_DATA_UART2);
	int gb_rsp = GB_RFR_UART2("SEND OK",2000);
	if (gb_rsp==1) GB_printString1(gb_RX_DATA_UART2);
	//printString1(RX_DATA_UART2);
	
	GB_printString1(gb_signMessage);
	webdata_debug = 0;
}
void GB_esp8266_webpageclose()
{
	GB_printString1(gb_RX_DATA_UART2);
	
	GB_printString1("\n****************Closing the session with Client**************\n");
	char gb_cipclose[20];
	sprintf(gb_cipclose,"AT+CIPCLOSE=%u\r\n",gb_channelid-48);
	//_delay_ms(10);
	
	GB_printString1("\nT\n");
	GB_send_command(gb_cipclose);
	GB_printString1("\nL\n");
	GB_printString1(gb_cipclose);
	int gb_rsp = GB_RFR_UART2("OK",1000);
	if (gb_rsp==1) GB_printString1(gb_RX_DATA_UART2);
	GB_printString1(gb_RX_DATA_UART2);
}



void GB_esp8266_iniserver()
{
	
	GB_send_command("AT+CWMODE?\r\n");
	GB_getstring_UART2();

	//Configure as Station
	GB_send_command("AT+CWMODE=1\r\n");
	GB_getstring_UART2();

	//Start Multiple connection
	GB_send_command("AT+CIPMUX=1\r\n");
	GB_getstring_UART2();


	GB_send_command("AT+CWJAP?\r\n");
	GB_getstring_UART2();


	//Connect To wifi
	//send_command("AT+CWJAP=\"JioFiber 2.4ghz\",\"Mansi5481\"\r\n");
	//RFR_UART2(16000);

	//Created A Server
	GB_send_command("AT+CIPSERVER=1,80\r\n");
	GB_RFR_UART2(">",1000);
}
