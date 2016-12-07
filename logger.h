/*
 * logger.h
 *
 *  Created on: Nov 24, 2016
 *      Author: Clint Olsen
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdint.h>
#include "gps.h"

GPS_Err log_gps_lock();
GPS_Err log_utc(uint8_t *time, int len);
GPS_Err log_date(uint8_t *date, int len);
GPS_Err log_latitude(uint8_t *latitude, uint8_t dir, int len);
GPS_Err log_longitude(uint8_t *longitude, uint8_t dir, int len);



#endif /* LOGGER_H_ */
