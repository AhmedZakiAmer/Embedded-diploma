/*
 * pir.c
 *
 *  Created on: Nov 3, 2024
 *      Author: ahmed
 */
#include "pir.h"
#include "avr/io.h"

void PIR_init(void){
	GPIO_setupPinDirection(PIR_PORT,PIR_PIN,PIN_INPUT);


}

uint8 PIR_getState(void){
	return GPIO_readPin(PIR_PORT,PIR_PIN);
}

