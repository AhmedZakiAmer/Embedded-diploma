/*
 * ultra_soinc.c
 *
 *  Created on: Oct 12, 2024
 *      Author: ahmed
 */

#include "ultra_sonic.h"
#include "avr/delay.h"

uint16 distance;
uint16 time;

void Ultrasonic_init(void){
	ICU_ConfigType ICU_Configurations ={F_CPU_8,RAISING};
	ICU_setCallBack(Ultrasonic_edgeProcessing);
	ICU_init(&ICU_Configurations);
	GPIO_setupPinDirection(PORTD_ID,PIN7_ID,PIN_OUTPUT);
}

void Ultrasonic_Trigger(void){
	GPIO_writePin(PORTD_ID,PIN7_ID,LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(PORTD_ID,PIN7_ID,LOGIC_LOW);
}

uint16 Ultrasonic_readDistance(void){
	Ultrasonic_Trigger();
	while (time ==0);
//	distance = (time*340) /2 ;
	distance = (time / 113.8) ;

	return distance;

}


void Ultrasonic_edgeProcessing(void) {
    static uint8 edge_count = 0;
    if (edge_count == 0) {
        ICU_clearTimerValue();
        ICU_setEdgeDetectionType(FALLING);
        edge_count++;
    } else {
        time = ICU_getInputCaptureValue();
        ICU_setEdgeDetectionType(RAISING);
        edge_count = 0;
    }
}


