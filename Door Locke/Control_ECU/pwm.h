/*
 * pwm.h
 *
 *  Created on: Oct 6, 2024
 *      Author: ahmed
 */

# include "gpio.h"
# include "std_types.h"

#define PWM_FAN_OFF  0
#define PWM_FAN_25  64
#define PWM_FAN_50  128
#define PWM_FAN_75  192
#define PWM_FAN_100  255

#define PWM_FAN_OUT_POART PORTB_ID
#define PWM_FAN_OUT_PIN   PIN3_ID


void PWM_Timer0_Start(uint8 duty_cycle);

