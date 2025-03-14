/*
 * Car_parking_sensor.c
 *
 *  Created on: Oct 12, 2024
 *      Author: ahmed
 */

#include "led.h"
#include "lcd.h"
#include "buzzer.h"
#include "avr/io.h"
#include "avr/delay.h"
#include "common_macros.h"

//#include "avr/delay.h"

int main(void) {
	uint16 distance;
	LCD_init();
	LEDS_init();
	Buzzer_init();
	Ultrasonic_init();
	SET_BIT(SREG, SREG_I);
	for (;;) {
		distance = Ultrasonic_readDistance();
		LCD_displayStringRowColumn(0, 1, "Distance =");
		LCD_moveCursor(0, 11);
		LCD_intgerToString(distance);
//		_delay_ms(50);
		if (distance < 100) {
			if (distance < 10) {
				LCD_displayStringRowColumn(0, 12, " ");
			}
			LCD_displayStringRowColumn(0, 13, " ");
		}
		LCD_displayStringRowColumn(0, 14, "cm");
		if (distance <= 5) {
			Buzzer_on();
			LED_on(red);
			LED_on(blue);
			LED_on(green);
			LCD_displayStringRowColumn(1, 3, "Stop");
		} else if (distance >= 6 && distance <= 10) {
			LED_on(blue);
			LED_on(red);
			LED_on(green);
			Buzzer_off();
			LCD_displayStringRowColumn(1, 3, "    ");
		} else if (distance >= 11 && distance <= 15) {
			LED_on(green);
			LED_on(red);
			LED_off(blue);
			Buzzer_off();
			LCD_displayStringRowColumn(1, 3, "    ");
		} else if (distance >= 16 && distance <= 20) {
			LED_on(red);
			LED_off(blue);
			LED_off(green);
			Buzzer_off();
			LCD_displayStringRowColumn(1, 3, "    ");
		} else {
			LED_off(red);
			LED_off(blue);
			LED_off(green);
			Buzzer_off();
			LCD_displayStringRowColumn(1, 3, "    ");
		}

	}
}
