/*
 * buzzer.h
 *
 *  Created on: Oct 8, 2024
 *      Author: ahmed
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"
#define BUZZER_PORT  PORTD_ID
#define BUZEER_PIN  PIN3_ID

void Buzzer_init(void);
void Buzzer_on(void);
void Buzzer_off(void);

#endif /* BUZZER_H_ */

