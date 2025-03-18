/*
 * Timer.h
 *
 *  Created on: Nov 3, 2024
 *      Author: ahmed
 */

#ifndef TIMER_H_
#define TIMER_H_

# include "std_types.h"
typedef enum
{
	TIMER0,TIMER1,TIMER2
}Timer_ID_Type;

typedef enum
{
	NO_CLOCK,F_CPU_1,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_ClockType;

typedef enum
{
	NORMAL_MODE,COMPARE_MODE
}Timer_ModeType;


typedef struct
{
uint16 timer_InitialValue;
uint16 timer_compare_MatchValue;     /*it will be used in compare mode only*/
Timer_ID_Type  timer_ID;
Timer_ClockType timer_clock;
Timer_ModeType  timer_mode;
}Timer_ConfigType;


void Timer_init(const Timer_ConfigType * Config_Ptr);
void Timer_deInit(Timer_ID_Type timer_type);
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID );

#endif /* TIMER_H_ */
