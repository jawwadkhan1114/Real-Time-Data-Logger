//***************************************************************************
//
// File Name            : ADC.h
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


#ifndef ADC_H_
#define ADC_H_

//function definitions for adc
void init_adc_temp();
void poll_adc();

//store value of converted adc result
uint16_t adc_conv_result;
uint16_t res_result;
uint8_t bcd_values[3];
int negative_flag;

#endif /* ADC_H_ */