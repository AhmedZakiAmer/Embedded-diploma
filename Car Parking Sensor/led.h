/*
 * led.h
 *
 *  Created on: Oct 7, 2024
 *      Author: ahmed
 */

#ifndef LED_H_
#define LED_H_

#include "gpio.h"

#define RED_PORT  PORTC_ID
#define RED_PIN PIN0_ID
#define GREEN_PORT  PORTC_ID
#define GREEN_PIN PIN1_ID
#define BLUE_PORT  PORTC_ID
#define BLUE_PIN PIN2_ID

#define POSTIVE_LOGIC
#ifdef POSTIVE_LOGIC
#define LED_ON LOGIC_HIGH
#define LED_OFF LOGIC_LOW
#else
#define LED_ON LOGIC_LOW
#define LED_OFF LOGIC_HIGH
#endif

typedef enum{
	red,green,blue
}LED_ID;

void LEDS_init(void);
void LED_on(LED_ID id);
void LED_off(LED_ID id);


#endif /* LED_H_ */
