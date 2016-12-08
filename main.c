/* Author: Clint Olsen
 * 12-7-16
 * Test Program to test motors
 */

#include "msp.h"
#include "gps.h"
#include "uart.h"
#include "motor.h"
#include "locator.h"
#include "logger.h"
#include "menu.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void EUSCIA0_IRQHandler(void);
void EUSCIA2_IRQHandler(void);
void TA0_0_IRQHandler(void);

//collect full NMEA string for processing
uint8_t NMEA_GPRMC_string[100];
int gps_on = 0;
int NMEA_count = 0; //count of characters in NMEA string
int capture_string = 0; //start storing the string
int process_string = 0; //flag to start processing the string

int gather_input = 0; //flag to start gathering input
int gather_gha = 0;
int count_gha = 0;
int gather_declination = 0;
int count_declination = 0;
int found_neg = 0; //indicates the number is negative


int begin_calculations = 0; // start location calculations
int begin_alt_motor = 0; //begin rotation of motor for altitude.
int begin_azi_motor = 0; //begin rotation of motor for altitude.
int switch_to_azi = 0; //transition flag between alt and azi motor


//timer variables
int step_counter = 0;

//data holding structures for gps and location information
gps_data current_info;
location_data location_info;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
    configure_serial_port0();
    //configure_pins();
    //configure_serial_port2();
    gps_on = 1;
    while(1){
    	//NMEA string processing state
    	if(process_string == 1){
    		int i;
    		//uart_putchar_n(NMEA_GPRMC_string,NMEA_count);
    		//uart_putchar(13);
    		for(i = 0; i < NMEA_count; i++){
    			sort_gps_data(&current_info, NMEA_GPRMC_string[i]);
    		}
    		process_string = 0;
    		NMEA_count = 0;
    		capture_string = 0;
    		//now that the data is collected, enable the bluetooth for input.
    		configure_serial_port2();
    		//turn on indicator led
    		gps_lock_led();
    		//begin_calculations = 1;
    		gather_input = 1;
    		gather_gha = 1;
    	}
    	//Bluetooth input gathering state
		if (gather_input == 1) {
			print_gha_menu();
			//wait until all data is gathered
			while (gather_gha == 1);
			log_gathered_gha(location_info.gha);
			print_dec_menu();
			//wait until all data is gathered
			while (gather_declination == 1);
			log_gathered_dec(location_info.declination);
			//convert input to usable numbers (doubles)
			gha_dec_atoi(&location_info, location_info.gha, 1);
			gha_dec_atoi(&location_info, location_info.declination, 0);
			begin_calculations = 1;
			gather_input = 0;
		}
    	//calculate altitude and azimuth of the desired object state
    	if(begin_calculations == 1){
    		calculate_lha(&location_info, location_info.gha_num, current_info.longitude_num, current_info.longitude_direction);
    		calculate_alt(&location_info, current_info.latitude_num, current_info.latitude_direction, location_info.lha, location_info.declination_num);
    		calculate_azi(&location_info, current_info.latitude_num, current_info.latitude_direction, location_info.lha, location_info.declination_num);
    		calculate_alt_steps(&location_info);
    		calculate_azi_steps(&location_info);
    		begin_calculations = 0;
    		begin_alt_motor = 1;
    	}

    	//begin rotating altitude motor state
    	if(begin_alt_motor == 1){
    		log_movement();
    		configure_timer();
    		configure_pins();
    		start_alt_motor();
    		begin_alt_motor = 0;
    	}
    	//begin rotating azimuth motor state
    	if(begin_azi_motor == 1){
    		start_azi_motor();
    		begin_azi_motor = 0;
    	}
    }
}

void EUSCIA0_IRQHandler(void){
	uint8_t data;
	//recieve interrupts
	if (UCA0IFG & UCRXIFG){
		//code to handle RX interrupts
		//prints recived data back out to terminal
		data = UCA0RXBUF;

		if(data == '$'){
			capture_string = 1;
			NMEA_GPRMC_string[NMEA_count] = data;
			NMEA_count++;
		}
		//reached end of string.
		else if(capture_string == 1 && data == 10){
			if(NMEA_GPRMC_string[17] == 'A' || NMEA_GPRMC_string[18] == 'A'){ // != 'V'
				UCA0IE &= ~EUSCI_A__RXIE; // Disable USCI_A0 RX interrupts
				process_string = 1;
			}
			//process_string = 1;
		}
		else if(capture_string == 1){
			if(NMEA_count == 4){
				if(NMEA_GPRMC_string[3] != 'R'){
					capture_string = 0;
					NMEA_count = 0;
				}
				else{
					NMEA_GPRMC_string[NMEA_count] = data;
					NMEA_count++;
				}
			}
			else{
				NMEA_GPRMC_string[NMEA_count] = data;
				NMEA_count++;
			}
		}

	}
	//transmit interrupts
	if (UCA0IFG & UCTXIFG){
	}
}

void EUSCIA2_IRQHandler(void){

	uint8_t data;
	//data = UCA2RXBUF; //reading this clears flag
	//uart_putchar(data);
	//recieve interrupts
	if (UCA2IFG & UCRXIFG){
		data = UCA2RXBUF; //reading this clears flag
		if(gather_gha == 1){
			if(data == '-'){
				found_neg = 1;
			}
			location_info.gha[count_gha] = data;
			count_gha++;
			if(found_neg == 1 && count_gha == 6){
				gather_gha = 0;
				gather_declination = 1;
				found_neg = 0;
			}
			//full GHA is found, move to declination and stop gathering GHA
			else if(found_neg == 0 && count_gha == 5){
				gather_gha = 0;
				gather_declination = 1;
			}
		}
		else if(gather_declination == 1){
			if (data == '-') {
				found_neg = 1;
			}
			location_info.declination[count_declination] = data;
			count_declination++;
			if (found_neg == 1 && count_declination == 6) {
				gather_declination = 0;
				found_neg = 0;
			}
			//full GHA is found, move to declination and stop gathering GHA
			else if (found_neg == 0 && count_declination == 5) {
				gather_declination = 0;
			}
		}
	}

	//transmit interrupts
	if (UCA2IFG & UCTXIFG){

	}
}

void TA0_0_IRQHandler(void) {
	if(switch_to_azi == 0){
		P2OUT ^= BIT5; //toggle step pin;
		step_counter++;
		//stop altitude movement
		if(step_counter == 2*location_info.alt_steps){
			stop_alt_motor();
			step_counter = 0;
			begin_azi_motor = 1;
			switch_to_azi = 1;
		}
	}
	else if(switch_to_azi == 1){
		P3OUT ^= BIT6; //toggle step pin
		step_counter++;
		//stop azimuth movement
		if(step_counter == 2*location_info.azi_steps){
			log_object_found();
			stop_azi_motor();
			step_counter = 0;
			switch_to_azi = 0;
			toggle_laser();
		}
	}
	TA0CCTL0 &= ~CCIFG;
}


