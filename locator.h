/*
 * locator.h
 *
 *  Created on: Dec 5, 2016
 *      Author: Clint Olsen
 */

#ifndef LOCATOR_H_
#define LOCATOR_H_

#include <stdint.h>

typedef struct location_data_t{
	double lha;
	double altitude;
	double azimuth;
	uint16_t alt_steps;
	uint16_t azi_steps;
}location_data;

void calculate_lha(location_data *info, double gha, uint8_t longitude, uint8_t longitude_dir); // returns LHA
void calculate_alt(location_data *info, uint8_t latitude, uint8_t latitude_dir, double lha, double declination);
void calculate_azi(location_data *info, uint8_t latitude, uint8_t latitude_dir, double lha, double declination);
void calculate_alt_steps(location_data *info);
void calculate_azi_steps(location_data *info);



#endif /* LOCATOR_H_ */
