/*
 * Timer.c
 *
 *  Created on: Nov 3, 2024
 *      Author: ahmed
 */

#include "timer.h"
#include "avr/io.h"
#include "avr/interrupt.h"

static volatile void (*Timer0_callback)(void) = NULL_PTR;
static volatile void (*Timer1_callback)(void) = NULL_PTR;
static volatile void (*Timer2_callback)(void) = NULL_PTR;


ISR(TIMER0_OVF_vect) {
    if (Timer0_callback != NULL_PTR) {
        Timer0_callback();
    }
}

ISR(TIMER1_OVF_vect) {
    if (Timer1_callback != NULL_PTR) {
        Timer1_callback();
    }
}

ISR(TIMER2_OVF_vect) {
    if (Timer2_callback != NULL_PTR) {
        Timer2_callback();
    }
}

ISR(TIMER0_COMP_vect) {
    if (Timer0_callback != NULL_PTR) {
        Timer0_callback();
    }
}

ISR(TIMER1_COMPA_vect) {
    if (Timer1_callback != NULL_PTR) {
        Timer1_callback();
    }
}

ISR(TIMER2_COMP_vect) {
    if (Timer2_callback != NULL_PTR) {
        Timer2_callback();
    }
}


void Timer_init(const Timer_ConfigType *Config_Ptr) {
	switch (Config_Ptr->timer_ID) {
	case TIMER0:
		TCCR0 = 0;
		TCNT0 = Config_Ptr->timer_InitialValue;
		TCCR0 |= Config_Ptr->timer_clock;
		if (Config_Ptr->timer_mode == COMPARE_MODE) {
			TCCR0 |= (1 << WGM01);
			OCR0 = Config_Ptr->timer_compare_MatchValue;
			TIMSK |= (1 << OCIE0);
			break;
		}
		TIMSK |= (1 << TOIE0);
		break;

	case TIMER1:
		TCCR1A = 0;
		TCCR1B = 0xC0;
		TCNT1 = Config_Ptr->timer_InitialValue;
		TCCR1B |= Config_Ptr->timer_clock;
		if (Config_Ptr->timer_mode == COMPARE_MODE) {
			TCCR1B |= (1 << WGM12);
			OCR1A = Config_Ptr->timer_compare_MatchValue;
			TIMSK |= (1 << OCIE1A);
			break;
		}
		TIMSK |= (1 << TOIE1);
		break;

	case TIMER2:
		TCCR2 = 0;
		TCNT2 = Config_Ptr->timer_InitialValue;
		TCCR2 |= Config_Ptr->timer_clock;
		if (Config_Ptr->timer_mode == COMPARE_MODE) {
			TCCR2 |= (1 << WGM21);
			OCR2 = Config_Ptr->timer_compare_MatchValue;
			TIMSK |= (1 << OCIE2);
			break;
		}
		TIMSK |= (1 << TOIE2);
		break;
	}
}

void Timer_deInit(Timer_ID_Type timer_type) {
	switch (timer_type) {
	case TIMER0:
		TCCR0 = 0;
		TIMSK &= ~(1 << TOIE0);
		TIMSK &= ~(1 << OCIE0);
		Timer0_callback = NULL_PTR;
		break;

	case TIMER1:
		TCCR1A = 0;
		TCCR1B = 0xC0;
		TIMSK &= ~(1 << TOIE1);
		TIMSK &= ~(1 << OCIE1A);
		Timer1_callback = NULL_PTR;
		break;

	case TIMER2:
		TCCR2 = 0;
		TIMSK &= ~(1 << TOIE2);
		TIMSK &= ~(1 << OCIE2);
		Timer2_callback = NULL_PTR;
		break;
	}
}

void Timer_setCallBack(void (*a_ptr)(void), Timer_ID_Type a_timer_ID) {
	switch (a_timer_ID) {
	case TIMER0:
		Timer0_callback = a_ptr;
		break;

	case TIMER1:
		Timer1_callback = a_ptr;
		break;

	case TIMER2:
		Timer2_callback = a_ptr;
		break;
	}
}

