/*
 * HMI_ECU.c
 *
 *  Created on: Nov 3, 2024
 *      Author: ahmed
 */

#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "avr/io.h"
#include "timer.h"

uint8 pass_str[7];
uint8 re_pass_str[7];
volatile uint8 counter_time1 = 0;
volatile uint8 counter_time0= 0;

UART_ConfigType uart_var = { Data_bit_8, Disabled, stop_bit_1, Baud_9600 };
Timer_ConfigType timer1 = { 0, 39062, TIMER1, F_CPU_1024, COMPARE_MODE }; // 5s
Timer_ConfigType timer0 = { 0, 156, TIMER0, F_CPU_1024, COMPARE_MODE }; // 20ms

#define MC_READY 0x10
#define MC_NOT_READY 0x15
#define MC_3_NOT_READY 0x11

void take_pass_to_check(void);
void send_pass(void);
void take_new_pass(void);
void timer1_fn(void);
void TIMER0_fn(void);
void delay_timer0(uint8);
void delay_timer1(uint8);

int main() {
	uint8 byte;
	SREG |= (1 << SREG_I);
	LCD_init();
	UART_init(&uart_var);
	Timer_init(&timer1);

	take_new_pass();
	while (1) {
		byte = UART_recieveByte();
		if (byte == MC_READY) {
			break;
		} else if (byte == MC_NOT_READY) {
			take_new_pass();
		}
	}
	LCD_clearScreen();
	for (;;) {
		LCD_displayStringRowColumn(0, 0, "+ : open door");
		LCD_displayStringRowColumn(1, 0, "- : change pass");
		delay_timer0(1);
		if (KEYPAD_getPressedKey() == '+') {
			delay_timer0(5);
			UART_sendByte(MC_READY);
			while (UART_recieveByte() != MC_READY) {
			}
			delay_timer0(5);
			UART_sendByte(3);
			while (1) {
				take_pass_to_check();
				send_pass();
				byte = UART_recieveByte();
				if (MC_3_NOT_READY == byte) {
					LCD_clearScreen();
					LCD_displayStringRowColumn(0, 1, "system loced");
					LCD_displayStringRowColumn(1, 1, "wait 1 min");
					delay_timer1(60);
					LCD_clearScreen();
					break;
				} else if (byte == MC_NOT_READY) {
					continue;
				} else if (byte == MC_READY) {
					LCD_clearScreen();
					LCD_displayStringRowColumn(0, 1, "Door is unlock");
					delay_timer1(15);
					LCD_clearScreen();
					while (MC_READY == UART_recieveByte()) {
						LCD_displayStringRowColumn(0, 1, "wait for people");
						LCD_displayStringRowColumn(1, 1, "to enter");
//						delay_timer0(1);
					}
					LCD_clearScreen();
					LCD_displayStringRowColumn(0, 1, "Door is Locking");
					delay_timer1(15);
					LCD_clearScreen();
					break;
				}
			}
		} else if (KEYPAD_getPressedKey() == '-') {
			delay_timer0(5);
			UART_sendByte(MC_READY);
			while (UART_recieveByte() != MC_READY) {
			}
			delay_timer0(5);
			UART_sendByte(4);
			while (1) {
				take_pass_to_check();
				send_pass();
				byte = UART_recieveByte();
				if (MC_3_NOT_READY == byte) {
					LCD_clearScreen();
					LCD_displayStringRowColumn(0, 1, "system loced");
					LCD_displayStringRowColumn(1, 1, "wait 1 min");
					delay_timer1(60);
					LCD_clearScreen();
					break;
				} else if (byte == MC_NOT_READY) {
					continue;
				} else if (byte == MC_READY) {
					take_new_pass();
					while (1) {
						byte = UART_recieveByte();
						if (byte == MC_READY) {
							break;
						} else if (byte == MC_NOT_READY) {
							take_new_pass();
						}
					}
					LCD_clearScreen();
					break;
				}
			}
		}

	}
}

void take_new_pass(void) {
	uint8 var;
	LCD_clearScreen();
	LCD_displayString("Plz enter N pass:");
	LCD_moveCursor(1, 0);
	for (var = 0; var < 5; ++var) {
		pass_str[var] = KEYPAD_getPressedKey() + '0';
		LCD_displayCharacter('*');
		delay_timer0(11);
	}
	while (KEYPAD_getPressedKey() != 13)
		;
	LCD_clearScreen();
	delay_timer0(7);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Plz re-enter pass: ");
	LCD_displayStringRowColumn(1, 0, "same pass: ");
	LCD_moveCursor(1, 11);
	for (var = 0; var < 5; ++var) {
		re_pass_str[var] = KEYPAD_getPressedKey() + '0';
		LCD_displayCharacter('*');
		delay_timer0(11);
	}
	while (KEYPAD_getPressedKey() != 13)
		;
	delay_timer0(3);
	pass_str[5] = '#';
	re_pass_str[5] = '#';
	pass_str[6] = '\0';
	re_pass_str[6] = '\0';

	UART_sendByte(MC_READY);
	while (UART_recieveByte() != MC_READY) {
	}
	delay_timer0(5);
	UART_sendString(pass_str);
	while (UART_recieveByte() != MC_READY) {
	}
	delay_timer0(5);
	UART_sendString(re_pass_str);
}

void send_pass() {
	UART_sendByte(MC_READY);
	while (UART_recieveByte() != MC_READY) {
	}
	delay_timer0(5);
	UART_sendString(pass_str);
}
void take_pass_to_check(void) {
	uint8 var;
	LCD_clearScreen();
	LCD_displayString("Plz enter pass: ");
	LCD_moveCursor(1, 0);
	for (var = 0; var < 5; ++var) {
		pass_str[var] = KEYPAD_getPressedKey() + '0';
		LCD_displayCharacter('*');
		delay_timer0(11);
	}
	pass_str[5] = '#';
	pass_str[6] = '\0';
	while (KEYPAD_getPressedKey() != 13)
		;
	delay_timer0(2);
	;
}

void timer1_fn() {
	counter_time1 += 5;		// 5s
}
void TIMER0_fn() {
	counter_time0 += 1;		//20ms

}

void delay_timer0(uint8 delay0) {
	Timer_init(&timer0);
	Timer_setCallBack(TIMER0_fn, TIMER0);
	while (counter_time0 != delay0)
		;
	Timer_deInit(TIMER0);
	counter_time0 = 0;
}
void delay_timer1(uint8 delay1) {
	Timer_init(&timer1);
	Timer_setCallBack(timer1_fn, TIMER1);
	while (counter_time1 != delay1)
		;
	Timer_deInit(TIMER1);
	counter_time1 = 0;
}
