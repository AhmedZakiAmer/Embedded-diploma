/*
 * Interfacing1_Project.c
 *
 *  Created on: Sep 9, 2024
 *      Author: ahmed
 */
# include <avr/io.h>
# include <avr/interrupt.h>
# include <util/delay.h>

# define SET_BIT(REG,BIT_NUM)(REG |= (1<<BIT_NUM))
# define CLEAR_BIT(REG,BIT_NUM)(REG &= ~(1<<BIT_NUM))
# define TOGGLE_BIT(REG,BIT_NUM)(REG ^= (1<<BIT_NUM))
# define BIT_IS_SET(REG,BIT_NUM)(REG &(1<<BIT_NUM))
# define BIT_IS_CLEAR(REG,BIT_NUM)(!(REG &(1<<BIT_NUM)))
# define SET_PART(REG,HEX_NUM)(REG |= HEX_NUM)
# define CLEAR_PART(REG,HEX_NUM)(REG &= HEX_NUM)

unsigned char sec = 0, minute = 0, hour = 0;
char flag = 0, countFlag = 1;

ISR(TIMER1_COMPA_vect) {
	CLEAR_BIT(PORTD, 0);
	if (countFlag) {
		sec++;
		if (sec == 60) {
			sec = 0;
			minute++;
		}
		if (minute == 60) {
			minute = 0;
			hour++;
		}
	} else {
		if (sec == 0 && minute > 0) {
			minute--;
			sec = 59;
		} else if (sec == 0 && minute == 0 && hour > 0) {
			hour--;
			minute = 59;
			sec = 59;
		} else if (sec == 0) {
			SET_BIT(PORTD, 0);
			CLEAR_PART(TCCR1B, 0xF8);
		} else {
			sec--;
		}

	}
}
ISR(INT0_vect) {
	sec = 0;
	minute = 0;
	hour = 0;
}
ISR(INT1_vect) {
	if (BIT_IS_SET(PIND, 3)) {
		CLEAR_PART(TCCR1B, 0xF8); /*Stop timer*/
	}
}
ISR(INT2_vect) {
	SET_PART(TCCR1B, 0x05); /*Resume timer*/
}

void Timer1_CTC_Init(void);
void INT_Init(void);
void bits(void);
void enable_sev(void);
void check_pins(void);
int main(void) {
	bits();
	INT_Init();
	Timer1_CTC_Init();

	for (;;) {
		if ((BIT_IS_CLEAR(PINB, 0)) || (BIT_IS_CLEAR(PINB, 1))
				|| (BIT_IS_CLEAR(PINB, 3)) || (BIT_IS_CLEAR(PINB, 4))
				|| (BIT_IS_CLEAR(PINB, 5)) || (BIT_IS_CLEAR(PINB, 6))
				|| BIT_IS_CLEAR(PINB, 7)) /*(~PINB) not working !!?*/
		{
			enable_sev();
			check_pins();
		} else {
			enable_sev();
			flag = 0;
		}

	}
}

void bits(void) {
	SET_PART(DDRC, 0x0F); /* Make first 4 bits in PORTC for 7447*/
	CLEAR_PART(PORTC, 0xF0); /* Output value 0 */

	SET_PART(DDRA, 0x3F); /* Make first 6 bits in PORTA for enable 7 segment*/
	CLEAR_PART(PORTA, 0xC0); /* Disable all*/

	CLEAR_PART(DDRB, 0x00); /*Make PORTA as input port*/
	SET_PART(PORTB, 0xFF); /*Enable internal pull up resistor for PORTA*/

	CLEAR_BIT(DDRD, 2); /* Input INT0 (PD2) */
	SET_BIT(PORTD, 2); /*Enable internal pull up resistor for PD2*/

	CLEAR_BIT(DDRD, 3); /* Input INT1 (PD3) */

	CLEAR_BIT(DDRB, 2); /* Input INT2 (PB2) */
	SET_BIT(PORTB, 2); /*Enable internal pull up resistor for PB2*/

	SET_PART(DDRD, 0x30); /*PD4 & PD5 as output */
	CLEAR_PART(PORTD, 0xCF); /*Clear PD4 & PD5*/

	CLEAR_BIT(DDRB, 7); /*Toggle count*/

	SET_BIT(DDRD, 0); /*Buzzer*/
}

void Timer1_CTC_Init(void) {
	SET_BIT(TCCR1A, FOC1A); /* ENABLE REG TCCR1A*/

	SET_BIT(TCCR1B, WGM12); /* ENABLE REG TCCR1B*/
	SET_BIT(TCCR1B, CS12);
	SET_BIT(TCCR1B, CS10);

	TCNT1 = 0; /* Initial value */
	OCR1A = 15625; /* Compare value */
	SET_BIT(TIMSK, OCIE1A); /*Enable interrupt*/
	SET_BIT(SREG, SREG_I); /*Enable interrupt bit*/

}

void INT_Init(void) {
	SET_BIT(GICR, 6); /*Enable INT 0*/
	SET_BIT(MCUCR, ISC01); /*Falling edge*/

	SET_BIT(GICR, 7); /*Enable INT 1*/
	SET_BIT(MCUCR,ISC11); /*Rising edge*/
	SET_BIT(MCUCR,ISC10);

	SET_BIT(GICR, 5); /*Enable INT 2*/
	CLEAR_BIT(MCUCSR, ISC2); /*Falling edge*/
}

void enable_sev() {
	char i;
	for (i = 0; i < 6; ++i) {
		CLEAR_PART(PORTA,0xc0);
		switch (i) {
		case 0:
			PORTC = (PORTC & 0xF0) | (0x0F & (hour / 10));
			break;
		case 1:
			PORTC = (PORTC & 0xF0) | (0x0F & (hour % 10));
			break;
		case 2:
			PORTC = (PORTC & 0xF0) | (0x0F & (minute / 10));
			break;
		case 3:
			PORTC = (PORTC & 0xF0) | (0x0F & (minute % 10));
			break;
		case 4:
			PORTC = (PORTC & 0xF0) | (0x0F & (sec / 10));
			break;
		case 5:
			PORTC = (PORTC & 0xF0) | (0x0F & (sec % 10));
			break;
		}
		SET_BIT(PORTA, i);
		_delay_ms(5);
	}
	if (countFlag) {
		CLEAR_BIT(PORTD, 5);
		SET_BIT(PORTD, 4);
	} else {
		CLEAR_BIT(PORTD, 4);
		SET_BIT(PORTD, 5);
	}
}

void check_pins(void) {
	if (!flag) {
		if (BIT_IS_CLEAR(PINB, 7)) {
			countFlag ^= 1;
		}
		if (BIT_IS_CLEAR(PINB, 0)) {
			if (hour > 0) {
				hour--;
			}
		}
		if (BIT_IS_CLEAR(PINB, 1)) {
			hour++;
		}
		if (BIT_IS_CLEAR(PINB, 3)) {
			if (minute > 0) {
				minute--;
			} else if (hour > 0) {
				hour--;
				minute = 59;
			}
		}
		if (BIT_IS_CLEAR(PINB, 4)) {
			minute++;
			if (minute == 60) {
				hour++;
				minute = 0;
			}
		}
		if (BIT_IS_CLEAR(PINB, 5)) {
			if (sec > 0) {
				sec--;
			} else if (minute > 0) {
				minute--;
				sec = 59;
			}
		}
		if (BIT_IS_CLEAR(PINB, 6)) {
			sec++;
			if (sec == 60) {
				sec = 0;
				minute++;
				if (minute == 60) {
					hour++;
					minute = 0;
				}
			}
		}
		flag = 1;
	}
}

