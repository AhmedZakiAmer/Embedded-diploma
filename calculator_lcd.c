/*
 * calculator_lcd.c
 *
 *  Created on: Sep 30, 2024
 *      Author: ahmed
 */

#include "keypad.h"
#include "lcd.h"
#include "avr/delay.h"
sint16 result = 0;
void calculate(uint8, uint16, uint16);

int main(void) {
	uint8  operator, factor = 1;
	sint16 firstNum,secondNum;
	while (KEYPAD_getPressedKey() != 13)
		;
	_delay_ms(150);
	LCD_init();
	LCD_intgerToString(0);
	for (;;) {
		if (KEYPAD_getPressedKey() < 10) {
			firstNum = 0;
			while (KEYPAD_getPressedKey() < 10) {
				firstNum *= 10;
				firstNum += KEYPAD_getPressedKey();
				LCD_clearScreen();
				if (KEYPAD_getPressedKey() == 13) {
					LCD_clearScreen();
					LCD_intgerToString(0);
					_delay_ms(200);
					continue;
				}
				LCD_intgerToString(firstNum);
				_delay_ms(200);
			}
		} else {
			firstNum = result;
			LCD_clearScreen();
			LCD_intgerToString(firstNum);
		}

		operator = KEYPAD_getPressedKey();
		if (KEYPAD_getPressedKey() == 13) {
			LCD_clearScreen();
			LCD_intgerToString(0);
			_delay_ms(200);
			continue;
		} else if (KEYPAD_getPressedKey() == '=') {
			_delay_ms(200);
			calculate(operator, firstNum, 0);
			continue;
		}
		if (firstNum > 99) {
			LCD_moveCursor(0, 4);
		} else {
			LCD_moveCursor(0, 3);
		}
		LCD_displayCharacter(operator);
		_delay_ms(200);
		factor = 1;
		secondNum = 0;
		while (KEYPAD_getPressedKey() < 10) {
			secondNum *= 10;
			secondNum += (KEYPAD_getPressedKey() * factor);
			if (KEYPAD_getPressedKey() == 13) {
				LCD_clearScreen();
				LCD_intgerToString(0);
				_delay_ms(200);
				continue;
			}
			if (firstNum > 99) {
				LCD_moveCursor(0, 6);
			} else {
				LCD_moveCursor(0, 5);
			}
			LCD_intgerToString(secondNum);
			_delay_ms(200);
		}

		if (KEYPAD_getPressedKey() == 13) {
			LCD_clearScreen();
			LCD_intgerToString(0);
			_delay_ms(200);
			continue;
		} else if (KEYPAD_getPressedKey() == '=') {
			calculate(operator, firstNum, secondNum);
		}
	}
}

void calculate(uint8 operator, uint16 firstNum, uint16 secondNum) {
	switch (operator) {
	case '+':
		result = firstNum + secondNum;
		break;
	case '*':
		result = firstNum * secondNum;
		break;
	case '-':
		result = firstNum - secondNum;
		break;
	case '/':
		if (secondNum == 0) {
			LCD_clearScreen();
			LCD_displayString("Wrong division");
			LCD_moveCursor(1, 0);
			LCD_displayString("By Zero");
			result = firstNum;
			_delay_ms(150);
			return;
		}
		result = firstNum / secondNum;
		break;
	case '=':
		result = firstNum;
		LCD_moveCursor(1, 0);
		LCD_intgerToString(result);
		return;
	default:
		LCD_clearScreen();
		LCD_displayString("Wrong in input");
		LCD_moveCursor(1, 1);
		LCD_displayString("Try again");
		break;
	}
	LCD_moveCursor(0, 8);
	LCD_displayString("= ");
	LCD_moveCursor(0, 10);
	LCD_intgerToString(result);
	_delay_ms(150);
}
