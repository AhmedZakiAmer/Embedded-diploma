/*
 * SmartHome.c
 *
 *  Created on: Oct 6, 2024
 *      Author: ahmed
 */

#include "adc.h"
#include "lcd.h"
#include "ldr.h"
#include "led.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "avr/delay.h"
#include "lm35_sensor.h"
#include "motor.h"
#include "buzzer.h"
#include "flame_sensor.h"

int main(void) {
	uint16 ldrLightIntensity;
	uint8 lm35TemperatureValue;
	LCD_init();
	ADC_init();
	LEDS_init();
	DcMotor_Init();
	FlameSensor_init();
	Buzzer_init();
	SREG |= (1 << SREG_I);  // Enable global interrupts
	for (;;) {
		if (FlameSensor_getValue()) {
			_delay_ms(20);
			LCD_clearScreen();
			while (FlameSensor_getValue()) {
				Buzzer_on();
				LCD_moveCursor(0, 0);
				LCD_displayString("Critical alert!");
			}
			Buzzer_off();
			LCD_clearScreen();
		}
		lm35TemperatureValue = LM35_getTemperature();
		LCD_moveCursor(0, 3);
		if (lm35TemperatureValue >= 25) {
//			LCD_moveCursor(0, 3);
			LCD_displayString("FAN is ON ");
		} else {
//			LCD_moveCursor(0, 3);
			LCD_displayString("FAN is OFF");
		}
		LCD_displayStringRowColumn(1, 0, "Temp=");
		//			LCD_moveCursor(1, 5);
		LCD_intgerToString(lm35TemperatureValue);
		if (lm35TemperatureValue < 100) {
			LCD_moveCursor(1, 7);
			LCD_displayCharacter(' ');
		}
		if (lm35TemperatureValue >= 40) {
			DcMotor_Rotate(CW, PWM_FAN_100);
		} else if (lm35TemperatureValue >= 35) {
			DcMotor_Rotate(CW, PWM_FAN_75);
		} else if (lm35TemperatureValue >= 30) {
			DcMotor_Rotate(CW, PWM_FAN_50);
		} else if (lm35TemperatureValue >= 25) {
			DcMotor_Rotate(CW, PWM_FAN_25);
		} else {
			DcMotor_Rotate(CW, PWM_FAN_OFF);
		}
		ldrLightIntensity = LDR_getLightIntensity();
		LCD_displayStringRowColumn(1, 8, "LDR=");
//		LCD_moveCursor(1, 12);
		LCD_intgerToString(ldrLightIntensity);
		if (ldrLightIntensity < 100) {
			LCD_moveCursor(1, 14);
			LCD_displayCharacter(' ');
			if (ldrLightIntensity < 10) {
				LCD_moveCursor(1, 13);
				LCD_displayCharacter(' ');
			}
		}
		LCD_moveCursor(1, 15);
		LCD_displayCharacter('%');
		if (ldrLightIntensity >= 70) {
			LED_off(red);
			LED_off(green);
			LED_off(blue);
		} else if (ldrLightIntensity >= 50) {
			LED_off(green);
			LED_off(blue);
			LED_on(red);
		} else if (ldrLightIntensity >= 15) {
			LED_off(blue);
			LED_on(red);
			LED_on(green);
		} else {
			LED_on(red);
			LED_on(green);
			LED_on(blue);
		}
	}
}
