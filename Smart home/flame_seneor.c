/*
 * flame_seneor.c
 *
 *  Created on: Oct 8, 2024
 *      Author: ahmed
 */

#include "flame_sensor.h"

void FlameSensor_init(void){
	GPIO_setupPinDirection(FLAME_SENSOR_PORT,FLAME_SENSOR_PIN,PIN_INPUT);
}

uint8 FlameSensor_getValue(void){
	return GPIO_readPin(FLAME_SENSOR_PORT,FLAME_SENSOR_PIN);
}
