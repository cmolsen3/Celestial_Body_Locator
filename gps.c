/*
 * gps.c
 *
 *  Created on: Nov 16, 2016
 *      Author: Clint Olsen
 */
#include "gps.h"
#include "uart.h"
#include "logger.h"
#include "msp.h"


int start_search = 0; //to rectify odd starting point for latitude and date search in string


//message id collection
int collect_message_id = 0;
int count_message_id = 0;
uint8_t rmc_code[5] = {'G','P','R','M','C'};

//UTC Collection
int collect_utc = 0;
int count_utc = 0;

//Latitude Collection
int collect_latitude = 0;
int count_latitude = 0;
int collect_latitude_dir = 0;

//Longitude Collection
int collect_longitude = 0;
int count_longitude = 0;
int collect_longitude_dir = 0;

//Date Collection
int collect_date = 0;
int count_date = 0;
uint8_t prev =  0;



GPS_Err sort_gps_data(gps_data *info, uint8_t data) {

	//look for the correct message id in the NMEA string $GPRMC
	if (data == '$') {
		//set look for message id flag
		collect_message_id = 1;
	}
	//start collecting the 5 character message code to compare
	else if (collect_message_id == 1) {
		if (count_message_id < 5) {
			message_id[count_message_id] = data;
			count_message_id++;
		}
		else if (count_message_id >= 5) {
			int i;
			//check for correct message code
			for(i = 0; i < 5; i++){
				if(message_id[i] == rmc_code[i]);
				else{
					collect_message_id = 0;
					count_message_id = 0;
					return INVALID_MESSAGE_ID;
				}
			}
			if(collect_message_id == 1){
				log_gps_lock();
				count_message_id = 0;
				collect_message_id = 0;

				//indicate to start looking for UTC
				collect_utc = 1;
			}
		}
	}

	//collect utc time next from string
	else if(collect_utc == 1 && data != ','){
		if(count_utc < 6){
			info->utc[count_utc] = data;
			count_utc++;
		}
		//found the entire UTC
		else{
			collect_utc = 0;
			//indicate to start looking for Latitude
			collect_latitude = 1;
			//log data
			log_utc(info->utc, count_utc);
			count_utc = 0;
		}

	}

	//collect latitude next from string
	//look for next ASCII values in the integer number range that indicates start of latitude data
	else if(collect_latitude == 1){
		if(start_search == 1){
			//set delimiter as the period in latitude measurement. Second condition is for the fact 2 periods are encountered
			if(data == '.' && count_latitude != 0){
				info->latitude_len = (uint8_t)count_latitude;
				//indicate to stop collecting data for latitude
				collect_latitude = 0;
				//start looking for latitude direction
				collect_latitude_dir = 1;
				//convert string to usable number
				lat_long_atoi(info,info->latitude,info->latitude_len,1);
			}
			else if(data >= 48 && data <= 57){
				info->latitude[count_latitude] = data;
				count_latitude++;
			}
		}
		else{
			//all previous numerical sections have passed, so it is safe to start seaching for numbers again.
			if(data == ','){
				start_search = 1;
			}
		}
	}

	//collect latitude direction from string
	else if(collect_latitude_dir == 1){
		if(data == 'N' || data == 'S'){
			info->latitude_direction = data;
			//indicate to stop looking for direction
			collect_latitude_dir = 0;
			//indicate to start looking for longitude
			collect_longitude = 1;
			//log data
			log_latitude(info->latitude, info->latitude_direction, count_latitude);

		}
	}

	//collect longitude next from string
	//look for next ASCII values in the integer number range that indicates start of longitude data
	else if(collect_longitude == 1){
			//set delimiter as the period in longitude measurement.
			if(data == '.'){
				//record length
				info->longitude_len = (uint8_t)count_longitude;
				//indicate to stop collecting data for longitude
				collect_longitude = 0;
				//start looking for longitude direction
				collect_longitude_dir = 1;
				//convert string to usable number
				lat_long_atoi(info,info->longitude,info->longitude_len,0);

			}
			else if(data >= 48 && data <= 57){
				info->longitude[count_longitude] = data;
				count_longitude++;
			}
	}

	//collect latitude direction from string
	else if(collect_longitude_dir == 1){
		if(data == 'E' || data == 'W'){
			info->longitude_direction = data;
			//indicate to stop looking for direction
			collect_longitude_dir = 0;
			//indicate to start looking for date
			collect_date = 1;
			//log data
			log_longitude(info->longitude, info->longitude_direction, count_longitude);
			start_search = 0;

		}
	}

	//collect date information from string
	else if(collect_date == 1){
		if(data == ',' && prev == ','){
			start_search = 1;
		}
		else if(start_search == 1 && data != ','){
			info->date[count_date] = data;
			count_date++;
		}
		else if(start_search == 1 && data == ','){
			collect_date = 0;
			log_date(info->date, count_date);
		}
		prev = data;
	}

	return NO_ERROR;
}

//turn on led to indicate GPS lock
void gps_lock_led(void){
	P2DIR |= BIT1;
	P2OUT |= BIT1;
}

void lat_long_atoi(gps_data *info, uint8_t *string, uint8_t len, uint8_t lat_long){
	uint8_t value = 0;
	// the NMEA string is 4 characters if the left side of the decimal place is to the 10's position
	if(len == 4){
		value += (string[0] - 48)*10;
		value += (string[1] - 48);
	}
	// the NMEA string is 5 characters if the left side of the decimal place is to the 100's position
	else if(len == 5){
		value += (string[0] - 48)*100;
		value += (string[1] - 48)*10;
		value += (string[2] - 48);
	}
	if(lat_long == 1){
		info->latitude_num = value;
	}
	else{
		info->longitude_num = value;
	}

}


