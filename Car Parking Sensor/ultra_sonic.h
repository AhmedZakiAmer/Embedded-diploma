/*
 * ultra_soinc.h
 *
 *  Created on: Oct 12, 2024
 *      Author: ahmed
 */

#ifndef ULTRA_SONIC_H_
#define ULTRA_SONIC_H_
#include "gpio.h"
#include "icu.h"
//#include "std_types.h"

#define ULTRA_SONIC_TRIGGER_PORT  PORTD_ID
#define ULTRA_SONIC_TRIGGER_PIN  PIN7_ID

#define ULTRA_SONIC_ECHO_PORT  PORTD_ID
#define ULTRA_SONIC_ECHO_PIN   PIN6_ID

void Ultrasonic_init(void);
void Ultrasonic_Trigger(void);
uint16 Ultrasonic_readDistance(void);
void Ultrasonic_edgeProcessing(void);



#endif /* ULTRA_SONIC_H_ */
