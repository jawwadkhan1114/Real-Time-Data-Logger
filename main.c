/*
 * fsm_ui.c
 *
 * Created: 4/26/2021 10:36:06 PM
 * Author : jawwa
 */ 

#define F_CPU 4000000

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ADC.h"
#include "RS232.h"
#include "LCD_DOG.h"
#include "DS1306_RTC_drivers.h"
#include "fsm_ui.h"

char degree_sign = 0xDF;

int main(void)
{
    init_adc_temp();
	init_clock();
	init_lcd_dog();
	init_RS232();
	sei();
    while (1) 
    {
		asm volatile("nop"); 
    }
}
ISR(PORTC_PORT_vect){
	poll_clock();
	poll_adc();
	sprintf(display_buff1, "Time: %d%d:%d%d:%d%d", converted_time_values[5], converted_time_values[4], converted_time_values[3], converted_time_values[2], converted_time_values[1], converted_time_values[0]);
	if(negative_flag){
		sprintf(display_buff2, "Temp = -%d%d.%d%cC", bcd_values[2],bcd_values[1],bcd_values[0],degree_sign);
		}else{
		sprintf(display_buff2, "Temp = %d%d.%d%cC", bcd_values[2],bcd_values[1],bcd_values[0],degree_sign);
	}
	update_lcd_dog();
	PORTC.INTFLAGS = PIN2_bm;
	return;
}

ISR(USART3_RXC_vect){
	volatile unsigned char entered_char;
	entered_char = receive_char();
	_delay_us(30);
	send_char(entered_char);
	if(entered_char == 's'){
		fsm(present_state, s);
	}
	else if(entered_char == 'h'){
		fsm(present_state, h);
	}
	else if(entered_char == 'm'){
		fsm(present_state, m);
	}
	else if(entered_char >= '0' && entered_char <= '9'){
		entered_digit = entered_char & 0x0F;
		fsm(present_state, digit);
	}
	else if(entered_char == 0x0D){
		fsm(present_state, enter);
	}
	else{
		fsm(present_state, entered_char);
	}
}