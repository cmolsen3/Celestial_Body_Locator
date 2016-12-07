/*
 * uart.c
 *
 *  Created on: Nov 11, 2016
 *      Author: Clint Olsen
 */

#include "uart.h"
#include "msp.h"
#include <stdint.h>

//UART preprocessor directives
//Baud Rate
#define br0 0x13 //9600 Baud
#define br1 0x00
#define CR 0x0D //carriage return


//Configure the serial port for the reciever specifications
void configure_serial_port0(){
	UCA0CTLW0 |= UCSWRST;       // Put eUSCI in reset
	P1SEL0 |= BIT2;				//Rx Primary mode
	P1SEL1 &= ~BIT2;

	P1SEL0 |= BIT3;				//Tx Primary mode
	P1SEL1 &= ~BIT3;
	// Select Frame parameters and clock source
	UCA0CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;
	UCA0CTLW0 &= ~EUSCI_A_CTLW0_PEN; //parity disabled
	UCA0CTLW0 &= ~EUSCI_A_CTLW0_MODE0; // set to uart mode
	UCA0CTLW0 &= ~EUSCI_A_CTLW0_MODE1;
	UCA0CTLW0 &= ~EUSCI_A_CTLW0_MSB;//LSB first
	UCA0CTLW0 &= ~EUSCI_A_CTLW0_SEVENBIT;//8 bit character length
	UCA0CTLW0 &= ~EUSCI_A_CTLW0_SPB; //one stop bit one start bit is default
	UCA0MCTLW |= EUSCI_A_MCTLW_OS16;
	UCA0MCTLW |= 0x0080;
	UCA0MCTLW &= 0x00FF;
	UCA0BR0 = br0;    // Set Baud Rate
	UCA0BR1 = br1;
	UCA0CTLW0 &= ~UCSWRST;      // Initialize eUSCI
	UCA0IE |= EUSCI_A__RXIE; // Enable USCI_A0 RX interrupts

	//NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);
	NVIC_EnableIRQ(EUSCIA0_IRQn);
}

//configure UCA2 for Bluetooth input
void configure_serial_port2(void){

		UCA2CTLW0 |= UCSWRST;       // Put eUSCI in reset
		P3SEL0 |= BIT2;				//Rx Primary mode
		P3SEL1 &= ~BIT2;

		P3SEL0 |= BIT3;				//Tx Primary mode
		P3SEL1 &= ~BIT3;

		// Select Frame parameters and clock source
		UCA2CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;
		UCA2CTLW0 &= ~EUSCI_A_CTLW0_PEN; //parity disabled
		UCA2CTLW0 &= ~EUSCI_A_CTLW0_MODE0; // set to uart mode
		UCA2CTLW0 &= ~EUSCI_A_CTLW0_MODE1;
		UCA2CTLW0 &= ~EUSCI_A_CTLW0_MSB;//LSB first
		UCA2CTLW0 &= ~EUSCI_A_CTLW0_SEVENBIT;//8 bit character length
		UCA2CTLW0 &= ~EUSCI_A_CTLW0_SPB; //one stop bit one start bit is default
		UCA2MCTLW |= EUSCI_A_MCTLW_OS16;
		UCA2MCTLW |= 0x0080;
		UCA2MCTLW &= 0x00FF;
		UCA2BR0 = br0;    // Set Baud Rate
		UCA2BR1 = br1;
		UCA2CTLW0 &= ~UCSWRST;      // Initialize eUSCI
		UCA2IE |= EUSCI_A__RXIE; // Enable USCI_A1 RX interrupts

		//NVIC->ISER[1] = 1 << ((EUSCIA1_IRQn) & 31);
		NVIC_EnableIRQ(EUSCIA2_IRQn);
}


//sends a single 8 bit value to the tx buffer
void uart_putchar(uint8_t tx_data){
	while(!(UCA0IFG & UCTXIFG));// Blockuntil transmitter is ready
	UCA0TXBUF = tx_data;    // Load data onto buffer
}

//sends a array of 8 bit values to tx buffer
void uart_putchar_n(uint8_t * data, uint32_t length){
	// Code to iterate through transmit data
	uint32_t i;
	for(i = 0; i < length; i++){
		uart_putchar(data[i]);
	}


}





