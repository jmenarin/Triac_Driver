/*
 * triac_driver.c
 *
 *  Created on: Dez 27, 2023
 *      Author: João Pedro Menarin
 */

//

#include "triac_driver.h"

#define SEMICYCLE 0.008333
#define ANGLEMAX 180

static bool (*get_pin_zero)(void);
static void (*triac_on)(void);
static void (*triac_off)(void);
static zero_cross_detected verify_zero(void);

static bool start_triac = 0;
static uint8_t delay = 0;

/*This function is called in the code when the user wants to turn oon the TRIAC
The maximum angle is 170 degrees*/

void turn_on_triac(uint8_t angle)
{
	start_triac = 1;
	if (angle >= 170)
	{
		angle = 170;
	}
	delay = (((SEMICYCLE / ANGLEMAX) * 10000) * angle);
}

/*This function is called in the code when the user wants to turn off the TRIAC*/
void turn_off_triac(void)
{
	start_triac = 0;
	triac_off();
}

/*This function needs to be called in an interrupt every 100 microseconds, and no parameters need to be passed to it*/
void triac_tick_100us(void)
{
	static uint8_t counter_delay = 0;
	static zero_cross_detected state_zero = NOT_DETECTED;

	if (start_triac == 1)
	{

		if (verify_zero() == DETECTED)
		{
			counter_delay = delay;
		}

		if (counter_delay > 0)
		{
			counter_delay--;
			if (counter_delay == 0)
			{
				state_zero = NOT_DETECTED;
				triac_on();
			}
		}
		triac_off();
	}
}

/*This function is called by the code to indicate when the ZeroCross signal has passed through zero*/
zero_cross_detected verify_zero(void)
{
	static state_pin_zero state_pin_zero = LOW;
	static zero_cross_detected state_zero = NOT_DETECTED;
	if (get_pin_zero() == 1 && state_pin_zero == LOW)
	{
		state_zero = DETECTED;
		state_pin_zero = HIGH;
		return state_zero;
	}
	if (get_pin_zero() == 0 && state_pin_zero == HIGH)
	{
		state_zero = DETECTED;
		state_pin_zero = LOW;
		return state_zero;
	}

	state_zero = NOT_DETECTED;

	return state_zero;
}

/*This function needs to be called in the code at the beginning for configuring the ZeroCross and TRIAC pin settings;.
 *It is necessary to pass as parameters the function that reads the ZeroCross, the function to turn on the TRIAC, and the function to turn off the TRIAC.*/
void triac_config(bool (*zero_get_pin_func)(void), void (*triac_on_func)(void), void (*triac_off_func)(void))
{
	get_pin_zero = zero_get_pin_func;
	triac_on = triac_on_func;
	triac_off = triac_off_func;
}
