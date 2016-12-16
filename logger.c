/*
 * logger.c
 *
 */

#define CR 0x0D //carriage return
#define SP 0x20 //space
#include <stdint.h>
#include "logger.h"
#include "uart.h"
#include "locator.h"


GPS_Err log_gps_lock(){
	uint8_t message[9] = {'G','P','S',' ','L','o','c','k'};
	uart_putchar_n(message, 8);
	uart_putchar(CR);

	return NO_ERROR;
}

GPS_Err log_utc(uint8_t *time, int len){
	uint8_t message[11] = {'U','T','C',' ','T','i','m','e',':',' '};
	uart_putchar_n(message, 10);
	uart_putchar_n(time, len);
	uart_putchar(CR);

	return NO_ERROR;
}

GPS_Err log_latitude(uint8_t *latitude, uint8_t dir, int len){
	uint8_t message[11] = {'L','a','t','i','t','u','d','e',':',' '};
	uart_putchar_n(message, 10);
	uart_putchar_n(latitude, len);
	uart_putchar(SP);
	uart_putchar(dir);
	uart_putchar(CR);

	return NO_ERROR;
}

GPS_Err log_longitude(uint8_t *longitude, uint8_t dir, int len){
	uint8_t message[12] = {'L','o','n','g','i','t','u','d','e',':',' '};
	uart_putchar_n(message, 11);
	uart_putchar_n(longitude, len);
	uart_putchar(SP);
	uart_putchar(dir);
	uart_putchar(CR);

	return NO_ERROR;
}

GPS_Err log_date(uint8_t *date, int len){
	uint8_t message[7] = { 'D', 'a', 't', 'e', ':',' '};
	uart_putchar_n(message, 6);
	uart_putchar_n(date, len);
	uart_putchar(CR);

	return NO_ERROR;
}

GPS_Err log_movement(){
	uint8_t message[12] = { 'L', 'o', 'c', 'a', 't','i','n','g','.','.','.'};
	uart_putchar_n(message, 11);
	uart_putchar(CR);
	return NO_ERROR;
}

GPS_Err log_object_found(location_data *info){
	uint8_t message[26] = { 'O', 'b', 'j', 'e', 'c','t',' ','F','o','u','n','d',' ','a','t',' ','A','l','t','i','t','u','d','e',' '};
	uint8_t degrees[9] = {' ', 'd','e','g','r','e','e','s'};
	uart_putchar_n(message, 25);

	//convert altitude and azimuth to a uint8_t array and send via UART
	int hund;
	int tens;
	int ones;
	int tenths;
	int hundreths;
	uint8_t tmp[7];

	//find the digit in each place (10s, 100s...) and convert it to an ASCII value
	hund = (int)(info->altitude / 100.0);
	tmp[0] = (uint8_t)hund + 48; //ASCII conversion

	tens =(int)(info->altitude - hund*100)/10;
	tmp[1] = (uint8_t)tens + 48;

	ones = (int)(info->altitude - hund*100 - tens*10);
	tmp[2] = (uint8_t)ones + 48;
	tmp[3] = 0x2E;

	tenths = (info->altitude - hund*100 - tens*10 - ones)/.1;
	tmp[4] = (uint8_t)tenths + 48;

	hundreths = (info->altitude - hund*100 - tens*10 - ones - (double)((double)tenths/10))/.01;
	tmp[5] = (uint8_t)hundreths + 48;

	uart_putchar_n(tmp,6);
	uart_putchar_n(degrees,8);

	uint8_t message2[14] = { ' ', 'a', 'n', 'd', ' ','A','z','i','m','u','t','h',' '};
	uart_putchar_n(message2, 13);

	//repeat for azimuth
	hund = (int)(info->azimuth / 100.0);
	tmp[0] = (uint8_t) hund + 48;

	tens = (int) (info->azimuth - hund * 100) / 10;
	tmp[1] = (uint8_t) tens + 48;

	ones = (int) (info->azimuth - hund * 100 - tens * 10);
	tmp[2] = (uint8_t) ones + 48;
	tmp[3] = 0x2E;

	tenths = (info->azimuth - hund * 100 - tens * 10 - ones) / .1;
	tmp[4] = (uint8_t) tenths + 48;

	hundreths = (info->azimuth - hund * 100 - tens * 10 - ones - (double) ((double) tenths / 10)) / .01;
	tmp[5] = (uint8_t) hundreths + 48;

	uart_putchar_n(tmp,6);
	uart_putchar_n(degrees,8);
	uart_putchar(CR);
	return NO_ERROR;
}

GPS_Err log_gathered_gha(uint8_t *gha){
	uint8_t message[14] = { 'G', 'H', 'A', ' ', 'R','e','c','e','i','v','e','d',':'};
	uart_putchar_n(message, 13);
	uart_putchar_n(gha, 6);
	uart_putchar(CR);

	return NO_ERROR;
}

GPS_Err log_gathered_dec(uint8_t *dec){
	uint8_t message[22] = { 'D', 'e', 'c','l','i','n','a','t','i','o','n',' ','R','e','c','e','i','v','e','d',':'};
	uart_putchar_n(message, 21);
	uart_putchar_n(dec, 6);
	uart_putchar(CR);

	return NO_ERROR;
}





