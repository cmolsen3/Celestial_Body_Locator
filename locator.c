/*
 * locator.c
 *
 *  Created on: Dec 5, 2016
 *      Author: Clint Olsen
 */

#include <math.h>
#include "locator.h"
#include<stdint.h>
#define pi 3.14159
#define degree_per_step .05625

void calculate_lha(location_data *info, double gha, uint8_t longitude, uint8_t longitude_dir){
	double lha;
	if(longitude_dir == 'W'){
		 lha = gha - longitude;
	}
	else if(longitude_dir == 'E'){
		lha = gha + longitude;
	}
	lha = lha * pi / 180.0; //in radians
	info->lha = lha;
}

void calculate_alt(location_data *info, uint8_t latitude, uint8_t latitude_dir, double lha, double declination){
	double lat_rad = latitude *pi/180; //convert latitude to radians
	double dec_rad = declination * pi /180; //convert declination to radians
	double alt;
	if(latitude_dir == 'N'){
		alt = asin(sin(lat_rad)*sin(dec_rad)+cos(lat_rad)*cos(dec_rad)*cos(lha));
	}
	else if(latitude_dir == 'S'){
		alt = asin(sin(-lat_rad)*sin(dec_rad)+cos(-lat_rad)*cos(dec_rad)*cos(lha));
	}
	alt = alt * 180 / pi;
	info->altitude = alt; //in degrees
}

void calculate_azi(location_data *info, uint8_t latitude, uint8_t latitude_dir, double lha, double declination){
	double lat_rad = latitude *pi/180; //convert latitude to radians
	double dec_rad = declination * pi /180; //convert declination to radians
	double azi;
	if(latitude_dir == 'N'){
		azi = atan(-sin(lha)/(tan(dec_rad)*cos(lat_rad)-sin(lat_rad)*cos(lha)));
	}
	else if(latitude_dir == 'S'){
		azi = atan(-sin(lha)/(tan(dec_rad)*cos(-lat_rad)-sin(-lat_rad)*cos(lha)));
	}
	azi = azi * 180 / pi;
	info->azimuth = azi; //in degrees
}

void calculate_alt_steps(location_data *info){
	uint16_t steps;
	steps = (uint16_t)(info->altitude / degree_per_step);
	info->alt_steps = steps;
}
void calculate_azi_steps(location_data *info){
	uint16_t steps;
	steps = (uint16_t)(info->azimuth / degree_per_step);
	info->azi_steps = steps;
}



