/*
 * buzzer.c
 *
 *  Created on: Oct 8, 2024
 *      Author: ahmed
 */

#include "buzzer.h"

void Buzzer_init(void){
	GPIO_setupPinDirection(BUZZER_PORT,BUZEER_PIN,PIN_OUTPUT);

	GPIO_writePin(BUZZER_PORT,BUZEER_PIN,LOGIC_LOW);
}
void Buzzer_on(void){
	GPIO_writePin(BUZZER_PORT,BUZEER_PIN,LOGIC_HIGH);
}
void Buzzer_off(void){
	GPIO_writePin(BUZZER_PORT,BUZEER_PIN,LOGIC_LOW);
}
