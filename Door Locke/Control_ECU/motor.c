/*
 * motor.c
 *
 *  Created on: Oct 8, 2024
 *      Author: ahmed
 */

#include "motor.h"

void DcMotor_Init(void) {
	GPIO_setupPinDirection(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_IN2_PORT_ID, MOTOR_IN2_PIN_ID, PIN_OUTPUT);

	GPIO_writePin(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
	GPIO_writePin(MOTOR_IN2_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
}
void DcMotor_Rotate(DcMotor_State state, uint8 speed) {
	switch (state) {
	case Stop:
		GPIO_writePin(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR_IN2_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
		break;
	case CW:
		GPIO_writePin(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR_IN2_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_HIGH);
		break;
	case ACW:
		GPIO_writePin(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_HIGH);
		GPIO_writePin(MOTOR_IN2_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
		break;
	}
	PWM_Timer0_Start(speed);
}
