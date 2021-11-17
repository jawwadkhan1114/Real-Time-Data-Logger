/*
 * fsm_ui.c
 *
 * Created: 4/26/2021 10:37:41 PM
 *  Author: jawwa
 */ 
#include "fsm_ui.h"
const transition idle_transitions [] =
{
	//INPUT		NEXT_STATE		TASK
	{s,			set,			set_fn},
	{eol,		idle,			error_fn}
};
const transition set_transitions [] =
{
	//INPUT		NEXT_STATE		TASK
	{h,			hours,			set_hours_fn},
	{m,			minutes,		set_minutes_fn},
	{s,			seconds,		set_seconds_fn},
	{eol,		idle,			error_fn}
};
const transition hours_transitions [] =
{
	//INPUT		NEXT_STATE		TASK
	{digit,		hours,			digit_in_fn},
	{s,			set,			set_fn},
	{enter,		idle,			enter_hour_fn},
	{eol,		idle,			error_fn}
};
const transition minutes_transitions [] =
{
	//INPUT		NEXT_STATE		TASK
	{digit,		minutes,		digit_in_fn},
	{s,			set,			set_fn},
	{enter,		idle,			enter_minute_fn},
	{eol,		idle,			error_fn}
};
const transition seconds_transitions [] =
{
	//INPUT		NEXT_STATE		TASK
	{digit,		seconds,		digit_in_fn},
	{s,			set,			set_fn},
	{enter,		idle,			enter_second_fn},
	{eol,		idle,			error_fn}
};
const transition * ps_transitions_ptr[5] =
{
	idle_transitions,
	set_transitions,
	hours_transitions,
	minutes_transitions,
	seconds_transitions
};
void fsm(state ps, key keyval){
	int i;
	
	for(i = 0; (ps_transitions_ptr[ps][i].keyval != keyval) 
	&& (ps_transitions_ptr[ps][i].keyval != eol); i++);
	
	ps_transitions_ptr[ps][i].tf_ptr();
	
	present_state = ps_transitions_ptr[ps][i].next_state;
	
}