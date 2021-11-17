/*
 * fsm_ui.h
 *
 * Created: 4/26/2021 10:37:31 PM
 *  Author: jawwa
 */ 

#ifndef FSM_UI_H_
#define FSM_UI_H_

#define F_CPU 4000000
#include "RS232.h"
#include "DS1306_RTC_drivers.h"
#include "LCD_DOG.h"
#include "ADC.h"
#include <avr/io.h>
#include <util/delay.h>

void set_fn();
void set_hours_fn();
void set_minutes_fn();
void set_seconds_fn();

void digit_in_fn();
void enter_hour_fn();
void enter_minute_fn();
void enter_second_fn();
void error_fn();

typedef void (* task_fn_ptr) ();

typedef enum{
	s, h, m, digit, enter, eol
} key;

typedef enum{
	idle, set, hours, minutes, seconds
} state;

typedef struct{
	key keyval;
	state next_state;
	task_fn_ptr tf_ptr;
}  transition;

state present_state;
uint8_t entered_digit;

uint8_t entered_digitarr[4];

void fsm(state ps, key keyval);

#endif /* FSM_UI_H_ */