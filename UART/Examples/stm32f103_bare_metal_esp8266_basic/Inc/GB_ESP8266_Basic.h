/*
 * GB_ESP8266_Basic.h
 *
 *  Created on: 25-Jul-2021
 *      Author: kunal
 */

#ifndef GB_ESP8266_BASIC_H_
#define GB_ESP8266_BASIC_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "stm32f1xx.h"
#include <stdbool.h>
char gb_get_command[100];


/* reading from Interrupted Ring Buffer*/
		char  GB_getchar_UART2();            // <--->   Serial.read()

		void GB_getstring_UART2();

		char* GB_search_buffer(const char* gb_search);

		void GB_send_command(const char * gb_command);
		int GB_RFR_UART2(const char * gb_rsp,unsigned int gb_timeout);
	//	int GB_RFR_UART2(unsigned int gb_timeout);
		//Command ESP initialize functions in both client and web server

		void GB_esp8266_getmode();
		void GB_esp8266_connectiontype();
		void GB_esp8266_getAP();
		void GB_esp8266_setAP(const char * gb_ssid, const char * gb_pass);
		void GB_esp8266_disconnect();
		void GB_esp8266_IPadress();
		void GB_esp8266_listAP();
		void GB_esp8266_rst();
		void GB_esp8266_initialise_client();



#endif /* GB_ESP8266_BASIC_H_ */
