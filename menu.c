/*
 * menu.c
 *
 */

#include "menu.h"
#include "uart.h"
#define CR 0x0D //carriage return
#define SP 0x20 //space

void print_gha_menu(){
	uint8_t message[11] = {'I','n','p','u','t',' ','G','H','A',':'};
	uart_putchar_n(message, 10);
	uart_putchar(CR);

}

void print_dec_menu(){
	uint8_t message[11] = {'I','n','p','u','t',' ','D','e','c',':'};
	uart_putchar_n(message, 10);
	uart_putchar(CR);
}



