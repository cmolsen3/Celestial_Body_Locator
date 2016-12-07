/*
 * logger.c
 *
 *  Created on: Nov 24, 2016
 *      Author: Clint Olsen
 */

#define CR 0x0D //carriage return
#define SP 0x20 //space
#include <stdint.h>
#include "logger.h"
#include "uart.h"


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





