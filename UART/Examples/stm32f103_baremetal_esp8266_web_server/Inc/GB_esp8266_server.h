//************ESP Server functions**********************//

#ifndef GB_ESP8266_SERVER_H_
#define GB_ESP8266_SERVER_H_

int gb_channelid;

//Function to wait for client to browse Ip address on Browser
uint32_t GB_getstring_ESP8266FORSERVER(char* gb_IP);
//function to find length of data to send
//const char * GB_esp8266_webpagedatacommand(int gb_val);
//Function to send AT+CIPSEND command
void GB_esp8266_serverdatatoclientcipsend();
//void GB_esp8266_serverdatatoclientcipsend(int  gb_val);
void GB_esp8266_webpagedata();
void GB_esp8266_webpageclose();
void GB_esp8266_iniserver();

#endif
