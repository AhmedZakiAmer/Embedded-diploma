/*
 * led.c
 *
 *  Created on: Oct 7, 2024
 *      Author: ahmed
 */

#include "led.h"

void LEDS_init(void) {
	GPIO_setupPinDirection(RED_PORT, RED_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(GREEN_PORT, GREEN_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(BLUE_PORT, BLUE_PIN, PIN_OUTPUT);

	GPIO_writePin(RED_PORT, RED_PIN, LED_OFF);
	GPIO_writePin(GREEN_PORT, GREEN_PIN, LED_OFF);
	GPIO_writePin(BLUE_PORT, BLUE_PIN, LED_OFF);
}

void LED_on(LED_ID id) {
	switch (id) {
	case red:
		GPIO_writePin(RED_PORT, RED_PIN, LED_ON);
		break;
	case green:
		GPIO_writePin(GREEN_PORT, GREEN_PIN, LED_ON);
		break;
	case blue:
		GPIO_writePin(BLUE_PORT, BLUE_PIN, LED_ON);
		break;
	}
}

void LED_off(LED_ID id) {
	switch (id) {
	case red:
		GPIO_writePin(RED_PORT, RED_PIN, LED_OFF);
		break;
	case green:
		GPIO_writePin(GREEN_PORT, GREEN_PIN, LED_OFF);
		break;
	case blue:
		GPIO_writePin(BLUE_PORT, BLUE_PIN, LED_OFF);
		break;
	}
}
