/*
 * ldr.c
 *
 *  Created on: Oct 7, 2024
 *      Author: ahmed
 */

#include "ldr.h"
#include "adc.h"
#include "avr/io.h"
#include "avr/delay.h"

uint16 LDR_getLightIntensity(void) {
	ADC_readChannel(0);
	_delay_ms(5);
	GPIO_setupPinDirection(LDR_OUT_POART, LDR_OUT_PIN, PIN_INPUT);
	uint16 light_value = (uint16) (((uint32) adcvalue
			* LDR_SENSOR_MAX_LIGHT_INTENSITY * ADC_REF_VOLT_VALUE)\

			/ (ADC_MAXIMUM_VALUE * LDR_SENSOR_MAX_VOLT_VALUE));
	return light_value;
}
