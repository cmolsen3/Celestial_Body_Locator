/*
 * gps.h
 *
 *  Created on: Nov 16, 2016
 *      Author: Clint Olsen
 */

#ifndef GPS_H_
#define GPS_H_

#include<stdint.h>

typedef struct gps_data_t{
	uint8_t latitude[5];
	uint8_t latitude_len;
	uint8_t latitude_num; //holds number in degrees as opposed to ASCII string
	uint8_t latitude_direction;
	uint8_t longitude[5];
	uint8_t longitude_len;
	uint8_t longitude_num; //holds number in degrees as opposed to ASCII string
	uint8_t longitude_direction;
	uint8_t utc[6]; //UTC from $GPRMC is 6 chars long
	uint8_t date[6];
}gps_data;

typedef enum GPS_Err_t {
	NO_ERROR = 0,
	INVALID_MESSAGE_ID = -1,

} GPS_Err;

uint8_t message_id[5];


//this function breaks up the data sequentially, placing the needed items in gps_data structure
GPS_Err sort_gps_data(gps_data *info, uint8_t data);
void lat_long_atoi(gps_data *info, uint8_t *string, uint8_t len, uint8_t lat_long); //lat long function: 1 for latitude, 0 for longitude

void gps_lock_led(void);


#endif /* GPS_H_ */
