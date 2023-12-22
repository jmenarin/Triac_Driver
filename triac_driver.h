/*
 * triac_driver.h
 *
 *  Created on: Dez 27, 2023
 *      Author: João Pedro Menarin
 */

#ifndef INC_BRAKE_H_
#define INC_BRAKE_H_

#include "stm8s.h"


typedef enum{
	LOW,
	HIGH
} state_pin_zero;

typedef enum{
	DETECTED,
	NOT_DETECTED
} zero_cross_detected;

void triac_tick_100us(void);
void turn_off_triac(void);
void turn_on_triac(uint8_t angle);
void triac_config(bool (*zero_get_pin_func)(void), void (*triac_on_func)(void), void (*triac_off_func)(void));

#endif