/*
 * fsm_fn.c
 *
 * Created: 4/27/2021 4:17:31 PM
 *  Author: jawwa
 */ 
#include "fsm_ui.h"
#include <string.h>
#define read_seconds_reg 0x00
#define write_seconds_reg 0x80
#define read_minute_reg 0x01
#define write_minute_reg 0x81
#define read_hours_reg 0x02
#define write_hours_reg 0x82

void set_fn(){
	//send_char('s');
}
void set_hours_fn(){
	send_char(0x0A);
	send_char(0x0D);
	_delay_us(30);
	//read current clock values
	poll_clock();
	send_char('h');
	_delay_us(30);
	send_char('=');
	//send hours 10 place
	send_char(converted_time_values[5] + '0');
	_delay_us(30);
	//send hours 1 place
	send_char(converted_time_values[4] + '0');
	_delay_us(30);
	//send newline and enter key
	send_char(0x0A);
	send_char(0x0D);
}
void set_minutes_fn(){
	//send new line char
	send_char(0x0A);
	send_char(0x0D);
	_delay_us(30);
	//read current clock values
	poll_clock();
	send_char('m');
	_delay_us(30);
	send_char('=');
	//send hours 10 place
	send_char(converted_time_values[3] + '0');
	_delay_us(30);
	//send hours 1 place
	send_char(converted_time_values[2] + '0');
	_delay_us(30);
	//send newline and enter key
	send_char(0x0A);
	send_char(0x0D);	
}
void set_seconds_fn(){
	//enter new line character
	send_char(0x0A);
	send_char(0x0D);
	_delay_us(30);
	//read current clock values
	poll_clock();
	send_char('s');
	_delay_us(30);
	send_char('=');
	_delay_us(30);
	//send hours 10 place
	send_char(converted_time_values[1] + '0');
	_delay_us(30);
	//send hours 1 place
	send_char(converted_time_values[0] + '0');
	_delay_us(30);
	//send newline and enter key
	send_char(0x0A);
	send_char(0x0D);	
}

void digit_in_fn(){
	entered_digitarr[3] = entered_digitarr[2];
	entered_digitarr[2] = entered_digitarr[1];
	entered_digitarr[1] = entered_digitarr[0];
	entered_digitarr[0] = entered_digit;
}
void enter_hour_fn(){
	//enter new line character
	send_char(0x0A);
	send_char(0x0D);
	_delay_us(30);
	uint8_t tempchar10 = entered_digitarr[1] << 4;
	uint8_t tempchar1 = entered_digitarr[0];
	uint8_t combinedchar = tempchar1 | tempchar10;
	write_RTC(write_hours_reg, combinedchar);	
}
void enter_minute_fn(){
	//enter new line character
	send_char(0x0A);
	send_char(0x0D);
	_delay_us(30);
	uint8_t tempchar10 = entered_digitarr[1] << 4;
	uint8_t tempchar1 = entered_digitarr[0];
	uint8_t combinedchar = tempchar1 | tempchar10;
	write_RTC(write_minute_reg, combinedchar);	
}
void enter_second_fn(){
	//enter new line character
	send_char(0x0A);
	send_char(0x0D);
	_delay_us(30);
	uint8_t tempchar10 = entered_digitarr[1] << 4;
	uint8_t tempchar1 = entered_digitarr[0];
	uint8_t combinedchar = tempchar1 | tempchar10;
	write_RTC(write_seconds_reg, combinedchar);	
}
void error_fn(){
	//enter new line character
	send_char(0x0A);
	send_char(0x0D);
	_delay_us(30);
	char* error_string = "unknown char entered";
	int error_length = strlen(error_string);
	for(int i = 0; i < error_length; i++){
		send_char(error_string[i]);
		_delay_us(30);
	}
	//enter new line character
	send_char(0x0A);
	send_char(0x0D);
	_delay_us(30);
}