/*
 * motor.h
 *
 *  Created on: Oct 8, 2024
 *      Author: ahmed
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#include "gpio.h"
#include "pwm.h"
#define MOTOR_IN1_PORT_ID  PORTB_ID
#define MOTOR_IN1_PIN_ID   PIN0_ID
#define MOTOR_IN2_PORT_ID  PORTB_ID
#define MOTOR_IN2_PIN_ID   PIN1_ID
#define MOTOR_EN_PORT_ID  PORTB_ID
#define MOTOR_EN_PIN_ID   PIN3_ID

typedef enum{
	CW, ACW, Stop
}DcMotor_State;

void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* MOTOR_H_ */
