/*
 * ldr.h
 *
 *  Created on: Oct 7, 2024
 *      Author: ahmed
 */

#ifndef LDR_H_
#define LDR_H_

#include "std_types.h"
#include "gpio.h"

#define LDR_SENSOR_CHANNEL_ID 0
#define LDR_SENSOR_MAX_VOLT_VALUE 2.56
#define LDR_SENSOR_MAX_LIGHT_INTENSITY 100
//#define LDR_LIGHT_INTENSITY_70 70
//#define LDR_LIGHT_INTENSITY_50 50
//#define LDR_LIGHT_INTENSITY_15 15
#define LDR_OUT_POART PORTA_ID
#define LDR_OUT_PIN   PIN1_ID

uint16 LDR_getLightIntensity(void);

#endif /* LDR_H_ */
