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
	if(azi < 0){
		azi = azi + 360;
	}
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

void gha_dec_atoi(location_data *info, uint8_t *string, uint8_t gha_or_dec){
	double deg = 0;
	double min = 0;
	double data = 0;
	//if value is negative
	if(string[0] == '-'){
		//3 characters for degrees
		deg += (string[1] - 48)*100;
		deg += (string[2] - 48)*10;
		deg += (string[3] - 48);

		//2 characters for minutes
		min += (string[4] - 48)*10;
		min += (string[5] - 48)*1;
		min = min/60;
		data = -1*(deg+min);
	}
	//if value is positive
	else{
		//3 characters for degrees
		deg += (string[0] - 48) * 100;
		deg += (string[1] - 48) * 10;
		deg += (string[2] - 48);

		//2 characters for minutes
		min += (string[3] - 48) * 10;
		min += (string[4] - 48) * 1;
		min = min/60;
		data = (deg + min);
	}

	if(gha_or_dec == 1){
		info->gha_num = data;
	}
	else if(gha_or_dec == 0){
		info->declination_num = data;
	}

}

void convert_alt(location_data *info){
	double temp;
	int i;
	temp = info->altitude/100;
	for(i = 0; i < 6; i++){
		if(i == 3){
			info->alt[i] = 0x2E;
		}
		else{
		info->alt[i] = (uint8_t)temp;
		temp = (temp - info->alt[i])*10;
		info->alt[i] += 48;
		}
	}
}

void convert_azi(location_data *info){
	double temp;
	int i;
	temp = info->azimuth/100.0;
	for(i = 0; i < 6; i++){
		if(i == 3){
			info->azi[i] = 0x2E;
		}
		else{
			info->azi[i] = (uint8_t)temp;
			temp = (temp - info->azi[i])*10;
			info->alt[i] += 48;
		}
	}
}

