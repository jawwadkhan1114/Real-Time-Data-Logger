/*
 * DS1306_RTC_drivers.c
 *
 * Created: 4/20/2021 5:02:46 PM
 *  Author: jawwa
 */ 
#include "DS1306_RTC_drivers.h"
//bitmasks

//spi 
#define intflag_bm 0x80
#define spi_mode_bm 0x01
//control register
#define read_control_reg 0x0F
#define write_control_reg 0x8F
#define wp_bm 0x40
#define one_hz_bm 0x04
#define alarm0_int_bm 0x01

//status register
#define read_status_reg 0x10
#define write_status_reg 0x90

//seconds register
//bit 6-4 are 10s place of seconds 0-5
//bit 3-0 are 1s place of seconds 0-9
#define read_seconds_reg 0x00
#define write_seconds_reg 0x80

//minute register
//bit 6-4 are 10s place of minute 0-5
//bit 3-0 are 1s place of minute 0-9
#define read_minute_reg 0x01
#define write_minute_reg 0x81

//hour register
//bit 6 controls 12 hr vs 24 hr
//bit 6 = 0, 24 hr mode
//bit 6 = 1, 12 hr mode
//if 24 hr mode bit
//bit 5-4 are 10s place of hours
//if 12 hr mode
//bit 5 is am or pm
#define read_hours_reg 0x02
#define write_hours_reg 0x82

//day register
//bit 2-0 hold the day
//number corresponding to date is user defined
#define read_day_reg 0x03
#define write_day_reg 0x83

//date register 
//bit 5-4 hold 10s place of date 0-3
//bit 3-0 hold 1s place of date 0-9
#define read_date_reg 0x04
#define write_date_reg 0x84

//year register 
//bit 7-4 hold 10s place of year 0-9
//bit 3-0 hold 1s place of year 0-9
#define read_year_reg 0x06
#define write_year_reg 0x86

//alarm register int0

//alarm seconds
#define read_alarm_sec 0x07
#define write_alarm_sec 0x87

//alarm minutes
#define read_alarm_min 0x08
#define write_alarm_min 0x88

//alarm hours
#define read_alarm_hour 0x09
#define write_alarm_hour 0x89

//alarm days
#define read_alarm_day 0x0A
#define write_alarm_day 0x8A

//user ram
#define read_user_ram 0x20
#define write_user_ram 0xA0

//******************************************************************************
// Function : void SPI_rtc_ds1306_config (void)
// Date and version : 041221, version 1.0
// Target MCU : AVR128 @ 4MHz
// Author : Ken Short
// DESCRIPTION
// This function unselects the ds_1306 and configures an AVR128DB48 operated at
// 4 MHz to communicate with the ds1306. Pin PC1 of the AVR128DB48 is used to
// select the ds_1306. SCLK is operated a the maximum possible frequency for
// the ds1306.
//******************************************************************************

void SPI_rtc_ds1306_config(){
	//enable pc1 as output for slave select
	PORTC.DIRSET = PIN1_bm;
	//unassert slave select (must be 1 for read and writes)
	PORTC.OUTCLR = PIN1_bm;
	
	//enable port a pa6 = sck, pa5 = miso, pa4 = mosi
	PORTA.DIRSET = PIN6_bm | PIN4_bm;
	PORTA.DIRCLR = PIN5_bm;
	
	//enable spi as master, turn spi on
	SPI0.CTRLA = SPI_MASTER_bm | SPI_ENABLE_bm;
	SPI0.CTRLB = SPI_SSD_bm | spi_mode_bm;
	
}

//***************************************************************************
// Function :
// void write_RTC (unsigned char reg_RTC, unsigned char data_RTC)
//
// Target MCU : AVR128DB48 @ 4MHz
// Target Hardware ;
// Author : Ken Short
// DESCRIPTION
// This function writes data to a register in the RTC. To accomplish this, it
// must first write the register's address (reg_RTC) followed by writing the
// data (data_RTC). In the DS1306 data sheet this operation is called an SPI
// single-byte write.
//**************************************************************************

void write_RTC(unsigned char reg_RTC, unsigned char data_RTC){
	SPI_rtc_ds1306_config();
	//select slave select
	PORTC.OUTSET = PIN1_bm;
	
	//send register address
	SPI0.DATA = reg_RTC;
	//wait for data register
	while((SPI0.INTFLAGS & SPI_IF_bm) != SPI_IF_bm);
	//send data
	SPI0.DATA = data_RTC;
	//wait for data register
	while((SPI0.INTFLAGS & SPI_IF_bm) != SPI_IF_bm);
	
	//unselect slave select
	PORTC.OUTCLR = PIN1_bm;
}

//***************************************************************************
// Function Name :
// unsigned char read_RTC (unsigned char reg_RTC)
// Target MCU : AVR128DB48 @ 4MHz
// Author : Ken Short
// DESCRIPTION
// This function reads data from a register in the RTC. To accomplish this, it
// must first write the register's address (reg_RTC) followed by writing a dummy
// byte to generate the SCLKs to read the data (data_RTC). In the DS1306 data
// sheet this operation is called am SPI single-byte read.
//**************************************************************************

unsigned char read_RTC(unsigned char reg_RTC){
	SPI_rtc_ds1306_config();
	
	//select slave select
	PORTC.OUTSET = PIN1_bm;
	uint8_t read_data, temp_dummy;
	//send register address
	temp_dummy = SPI0.DATA;
	SPI0.DATA = reg_RTC;
	//wait for data register
	while((SPI0.INTFLAGS & SPI_IF_bm) != SPI_IF_bm);
	//send dummy data
	temp_dummy = SPI0.DATA;
	SPI0.DATA = 0x00;
	//wait for data register
	while((SPI0.INTFLAGS & SPI_IF_bm) != SPI_IF_bm);
	
	//read in data
	read_data = SPI0.DATA;
	
	//unselect slave select
	PORTC.OUTCLR = PIN1_bm;
	
	return read_data;
}

//***************************************************************************
// Function Name : 
//"block_write_RTC"
// void block_write_RTC (volatile unsigned char *array_ptr,
// unsigned char strt_addr, unsigned char count)
// Target MCU : AVR128DB48 @ 4MHz
// Author : Ken Short
// DESCRIPTION
// This function writes a block of data from an array to the DS1306. strt_addr
// is the starting address in the DS1306. count is the number of data bytes to
// be transferred and array_ptr is the address of the source array.
//**************************************************************************
void block_write_RTC (volatile unsigned char *array_ptr, unsigned char strt_addr, unsigned char count){
	//set wp to 0
	//write_RTC(write_control_reg, 0x00);
	SPI_rtc_ds1306_config();
	PORTC.OUTSET = PIN1_bm;
	uint8_t i = 0;
	//write start address
	SPI0.DATA = strt_addr;
	//wait for data register
	while((SPI0.INTFLAGS & intflag_bm) != intflag_bm);	
	while(i < count){
		SPI0.DATA = *(array_ptr + i);
		while((SPI0.INTFLAGS & intflag_bm) != intflag_bm);
		i++;
	}
	PORTC.OUTCLR = PIN1_bm;
}

//***************************************************************************
// Function Name :
// "block_read_RTC"
// void block_read_RTC (volatile unsigned char *array_ptr,
// unsigned char strt_addr, unsigned char count)
// Target MCU : AVR128DB48 @ 4MHz
// Author : Ken Short
// DESCRIPTION
// This function reads a block of data from the DS1306 and transfers it to an
// array. strt_addr is the starting address in the DS1306. count is the number
// of data bytes to be transferred and array_ptr is the address of the
// destination array.
//**************************************************************************
void block_read_RTC(volatile unsigned char *array_ptr, unsigned char strt_addr, unsigned char count){
	//set wp to 0
	//write_RTC(write_control_reg, 0x00);
	SPI_rtc_ds1306_config();
	PORTC.OUTSET = PIN1_bm;
	uint8_t i = 0;
	SPI0.DATA = strt_addr;
	while((SPI0.INTFLAGS & intflag_bm) != intflag_bm);
	while(i < count){
		//send dummy data
		SPI0.DATA = 0x00;
		while((SPI0.INTFLAGS & intflag_bm) != intflag_bm);
		*(array_ptr + i) = SPI0.DATA;
		i++;
	}
	PORTC.OUTCLR = PIN1_bm;
}


void init_clock(){

	//set bit 7 of days, hours, minutes, seconds for alarm
	//configure spi
	SPI_rtc_ds1306_config();
	//set wp to 0
	write_RTC(write_control_reg, 0x00);
	//enable 1 hz
	write_RTC(write_control_reg, one_hz_bm);
	
	//set pc2 as input
	PORTC.DIRCLR = PIN2_bm;
	//set pc2 as rising edge interrupts
	PORTC.PIN2CTRL = 0x02;
	//write value of current time to clock
	
	block_write_RTC(RTC_time_date_write, write_seconds_reg, 7);
}

void poll_clock(){
	block_read_RTC(RTC_time_date_read, read_seconds_reg, 7);
	
	uint8_t seconds = RTC_time_date_read[0];
	uint8_t seconds_ones = seconds & 0x0F;
	uint8_t seconds_tens = (seconds & 0xF0) >> 4;
	converted_time_values[0] = seconds_ones;
	converted_time_values[1] = seconds_tens;
	
	uint8_t minutes = RTC_time_date_read[1];
	uint8_t minutes_ones = minutes & 0x0F;
	uint8_t minutes_tens = (minutes & 0xF0) >> 4;
	converted_time_values[2] = minutes_ones;
	converted_time_values[3] = minutes_tens;
	
	uint8_t hours = RTC_time_date_read[2];
	uint8_t hours_ones = hours & 0x0F;
	uint8_t hours_tens = (hours & 0xF0) >> 4;
	converted_time_values[4] = hours_ones;
	converted_time_values[5] = hours_tens;
	
}