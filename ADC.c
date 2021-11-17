//***************************************************************************
//
// File Name            : ADC.c
// Title                :
// Date                 : 4/14/2021
// Version              : 1.0
// Target MCU           : AVR128DB48
// Target Hardware      ;
// Author               : Jawwad Khan
// DESCRIPTION
// Initialize ADC for temperature measurement.
//
// Warnings             :
// Restrictions         : none
// Algorithms           : none
// References           :
//
// Revision History     : Initial version
//
//
//**************************************************************************

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "ADC.h"
#include "LCD_DOG.h"
#include "RS232.h"

//adc bit masks
#define adc_8_samps 0x03
#define ain6_bm 0x06
#define adc_conv_bm 0x01

//convert value from adc
uint32_t volt_val = 2500;
uint32_t divide_val_2n = 4096;
uint32_t numsamples_val = 8;
uint32_t offset_val = 500;


//***************************************************************************
//
// Function Name        : init_adc_temp
// Date                 : 4/14/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Jawwad Khan
// DESCRIPTION
// Function to internal ADC0.
//
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************

void init_adc_temp(){
	//enable port d as input
	PORTD.DIR = ~PIN6_bm;
	//set voltage for adc
	VREF.ADC0REF = VREF_REFSEL_2V500_gc;
	//set control b to 8 samples
	//ADC0.CTRLB = adc_8_samps;
	//enable positive value at ain6
	ADC0.MUXPOS = ain6_bm;
	//enable adc
	ADC0.CTRLA = ADC_ENABLE_bm;
	//start a conversion
	ADC0.COMMAND = adc_conv_bm;
}
//***************************************************************************
//
// Function Name        : poll_adc
// Date                 : 4/14/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Jawwad Khan
// DESCRIPTION
// Function to poll internal adc until measusment is done.
// After reading, convert value into decimal celsius value.
//
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void poll_adc(){
	//wait for resrdy to be 1
	while((ADC0.INTFLAGS & ADC_RESRDY_bm) != ADC_RESRDY_bm){}
	
	//read value from adc
	res_result = ADC0.RES;
	//res_result = 0x500;
	volatile uint32_t numerator = volt_val * res_result;
	volatile uint32_t denominator = divide_val_2n;
	
	if(numerator/denominator > offset_val){
		adc_conv_result = (numerator/denominator) - offset_val;
		negative_flag = 0;
		}else{
		adc_conv_result = offset_val - (numerator/denominator);
		negative_flag = 1;
	}
	
	//convert to BCD values
	volatile uint8_t decimal = adc_conv_result % 10;
	volatile uint8_t ones = (adc_conv_result/10)-(adc_conv_result/100)*10;
	volatile uint8_t tens = (adc_conv_result/100);
	bcd_values[0] = decimal;
	bcd_values[1] = ones;
	bcd_values[2] = tens;
	
	//start another conversion
	ADC0.COMMAND = adc_conv_bm;
}