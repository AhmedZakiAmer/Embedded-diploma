/*
 * Control_ECU.c
 *
 *  Created on: Nov 3, 2024
 *      Author: ahmed
 */

#include "std_types.h"
#include "string.h"
#include "avr/interrupt.h"
#include "buzzer.h"
#include "external_eeprom.h"
#include "motor.h"
#include "pir.h"
#include "uart.h"
#include "twi.h"
#include "timer.h"
#include "pwm.h"

#define MC_READY 0x10
#define MC_NOT_READY 0x15
#define MC_3_NOT_READY 0x11

UART_ConfigType ECU_uart_var = { Data_bit_8, Disabled, stop_bit_1, Baud_9600 };
TWI_ConfigType Config_Ptr = { 0b00000010, TWI_FAST_MODE };
Timer_ConfigType timer1 = { 0, 39062, TIMER1, F_CPU_1024, COMPARE_MODE }; // 5s
Timer_ConfigType timer2 = { 0, 156, TIMER2, F_CPU_1024, COMPARE_MODE }; // 20ms
uint8 pass_str[7];
uint8 re_pass_str[7];
volatile uint8 counter_time1 = 0;
volatile uint8 counter_time2 = 0;

void delay_timer1(uint8);
void delay_timer2(uint8);
void timer2_fn();
void timer1_fn();
void take_new_pass(void);
void write_eebrom(void);
uint8 read_eebrom(void);
void three_check_MC(void);
uint8 check_pass(void);

int main() {
	uint8 i, case_num;
	SREG |= (1 << SREG_I);
	Buzzer_init();
	DcMotor_Init();
	PIR_init();
	TWI_init(&Config_Ptr);
	UART_init(&ECU_uart_var);

	take_new_pass();
	while (check_pass()) {
		take_new_pass();
	}
	for (;;) {
		while (UART_recieveByte() != MC_READY) {
		}
		delay_timer2(5);
		UART_sendByte(MC_READY);
		delay_timer2(1);
		case_num = UART_recieveByte();
		three_check_MC();
		for (i = 0; i < 3; i++) {
			delay_timer2(5);
			if (!read_eebrom()) {
				if (i == 2) {
					UART_sendByte(MC_3_NOT_READY);
					Buzzer_on();
					delay_timer1(60);
					Buzzer_off();
				} else {
					UART_sendByte(MC_NOT_READY);
					three_check_MC();
				}
			} else {
				UART_sendByte(MC_READY);
				if (case_num == 3) {
					DcMotor_Rotate(CW, PWM_FAN_100);
					delay_timer1(15);
					while (PIR_getState() == 1) {
						delay_timer2(1);
						UART_sendByte(MC_READY);
					}
					delay_timer2(1);
					UART_sendByte(MC_NOT_READY);
					DcMotor_Rotate(ACW, PWM_FAN_100);
					delay_timer1(15);
					DcMotor_Rotate(Stop, PWM_FAN_OFF);
					break;
				}
				if (case_num == 4) {
					take_new_pass();
					while (check_pass()) {
						take_new_pass();
					}
					break;
				}
			}
		}
	}
}

void take_new_pass(void) {
	while (UART_recieveByte() != MC_READY) {
	}
	delay_timer2(5);
	UART_sendByte(MC_READY);
	delay_timer2(1);
	UART_receiveString(pass_str);
	delay_timer2(5);
	UART_sendByte(MC_READY);
	delay_timer2(5);
	UART_receiveString(re_pass_str);
}
void write_eebrom(void) {
	int var;
	for (var = 0; var < 5; ++var) {
		EEPROM_writeByte(0x0311 + var, *(pass_str + var));
		delay_timer2(1);
	}
}
uint8 read_eebrom(void) {
	int var;
	for (var = 0; var < 5; ++var) {
		EEPROM_readByte(0x0311 + var, &re_pass_str[var]);
		delay_timer2(1);
	}
	re_pass_str[var] = '\0';
	if (!strcmp(pass_str, re_pass_str)) {
		return 1;
	}
	return 0;
}
void three_check_MC(void) {
	while (UART_recieveByte() != MC_READY) {
	}
	delay_timer2(5);
	UART_sendByte(MC_READY);
	delay_timer2(1);
	UART_receiveString(pass_str);
}
uint8 check_pass(void) {
//	delay_timer2(7);
	if (!strcmp(pass_str, re_pass_str)) {
		UART_sendByte(MC_READY);
		write_eebrom();
		return 0;
	} else {
		UART_sendByte(MC_NOT_READY);
		return 1;
	}
}

void timer1_fn() {
	counter_time1 += 5;		// 5s
}
void timer2_fn() {
	counter_time2 += 1;		//20ms
}

void delay_timer2(uint8 delay2) {
	Timer_init(&timer2);
	Timer_setCallBack(timer2_fn, TIMER2);
	while (counter_time2 != delay2)
		;
	Timer_deInit(TIMER2);
	counter_time2 = 0;
}
void delay_timer1(uint8 delay1) {
	Timer_init(&timer1);
	Timer_setCallBack(timer1_fn, TIMER1);
	while (counter_time1 != delay1)
		;
	Timer_deInit(TIMER1);
	counter_time1 = 0;
}
