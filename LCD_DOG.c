//***************************************************************************
//
// File Name            : LCD_DOG.c
// Title                :
// Date                 : 4/14/2021
// Version              : 1.0
// Target MCU           : AVR128DB48
// Target Hardware      ;
// Author               : Jawwad Khan
// DESCRIPTION
// Initialize SPI specific for DOG_LCD.
// Write data out to lcd.
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
#include "LCD_DOG.h"

//define bit masks
#define bank_bm 0x80
#define seqop_bm 0x20
#define intflag_bm 0x80
#define spimode_bm 0x03

//***************************************************************************
//
// Function Name        : init_lcd_dog
// Date                 : 4/14/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Jawwad Khan
// DESCRIPTION
// Initialize LCD. Writes out specific commands to initialize the LCD
//
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************

void init_lcd_dog(){
	
	init_spi_lcd();
	
	//delay for 40 ms
	_delay_ms(40);
	
	//func_set1:
	lcd_spi_transmit_CMD(0x39);
	_delay_us(30);
	
	//func_set2:
	lcd_spi_transmit_CMD(0x39);	//send function set #2
	_delay_us(30);	//delay for command to be processed


	//bias_set:
	lcd_spi_transmit_CMD(0x1E);	//set bias value.
	_delay_us(30);	//delay for command to be processed


	//power_ctrl:
	lcd_spi_transmit_CMD(0x55);	//~ 0x50 nominal for 5V
	//~ 0x55 for 3.3V (delicate adjustment).
	_delay_us(30);	//delay for command to be processed


	//follower_ctrl:
	lcd_spi_transmit_CMD(0x6C);	//follower mode on...
	_delay_ms(40);	//delay for command to be processed


	//contrast_set:
	lcd_spi_transmit_CMD(0x7F);	//~ 77 for 5V, ~ 7F for 3.3V
	_delay_us(30);	//delay for command to be processed


	//display_on:
	lcd_spi_transmit_CMD(0x0c);	//display on, cursor off, blink off
	_delay_us(30);	//delay for command to be processed


	//clr_display:
	lcd_spi_transmit_CMD(0x01);	//clear display, cursor home
	_delay_us(30);	//delay for command to be processed


	//entry_mode:
	lcd_spi_transmit_CMD(0x06);	//clear display, cursor home
	_delay_us(30);	//delay for command to be processed

}
//***************************************************************************
//
// Function Name        : lcd_spi_transmit_CMD
// Date                 : 4/14/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Jawwad Khan
// DESCRIPTION
// Function to write out commands to LCD_DOG.
//
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************

void lcd_spi_transmit_CMD(unsigned char cmd){
	//assert rs to 0 for command
	PORTC.OUT = ~PIN0_bm;
	//assert slave
	PORTA.OUT = ~PIN7_bm;
	
	SPI0.DATA = cmd;
	//wait for data register
	while((SPI0.INTFLAGS & intflag_bm) != intflag_bm);
	
	//unassert slave
	PORTA.OUT = PIN7_bm;
}
//***************************************************************************
//
// Function Name        : lcd_spi_transmit_DATA
// Date                 : 4/14/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Jawwad Khan
// DESCRIPTION
// Function to write out data to LCD_DOG.
//
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void lcd_spi_transmit_DATA(unsigned char data){
	//assert rs to 1 for data
	PORTC.OUT = PIN0_bm;
	//assert slave
	PORTA.OUT = ~PIN7_bm;
	
	SPI0.DATA = data;
	//wait for data register
	while((SPI0.INTFLAGS & intflag_bm) != intflag_bm);
	
	//unassert slave
	PORTA.OUT = PIN7_bm;
}
//***************************************************************************
//
// Function Name        : init_spi_lcd
// Date                 : 4/14/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Jawwad Khan
// DESCRIPTION
// Function to initialize SPI of micro controller
//
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void init_spi_lcd(){
	//enable port a
	PORTA.DIR = PIN7_bm | PIN6_bm | PIN4_bm;
	//enable port c
	PORTC.DIR = PIN0_bm;
	
	//unassert slave select
	PORTA.OUT = PIN7_bm;
	//enable spi as master, turn spi on
	SPI0.CTRLA = SPI_MASTER_bm | SPI_ENABLE_bm;
	SPI0.CTRLB = SPI_SSD_bm | spimode_bm;
}
//***************************************************************************
//
// Function Name        : update_lcd_dog
// Date                 : 4/14/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Jawwad Khan
// DESCRIPTION
// Function to update data of LCD_DOG with data from the char buffers
// Writes out to each individual line with each buffer
//
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void update_lcd_dog(){
	
	init_spi_lcd();
	//send line 1 to the lcd module
	lcd_spi_transmit_CMD(0x80);
	_delay_us(30);	//delay for command to be processed
	for(int i = 0; i < 16; i++){
		lcd_spi_transmit_DATA(display_buff1[i]);
		_delay_us(30);	//delay for command to be processed
	}
	
	//send line 2 to the lcd module
	lcd_spi_transmit_CMD(0x90);
	_delay_us(30);	//delay for command to be processed
	for(int i = 0; i < 16; i++){
		lcd_spi_transmit_DATA(display_buff2[i]);
		_delay_us(30);	//delay for command to be processed
	}

	//send line 3 to the lcd module
	lcd_spi_transmit_CMD(0xA0);
	_delay_us(30);	//delay for command to be processed
	for(int i = 0; i < 16; i++){
		lcd_spi_transmit_DATA(display_buff3[i]);
		_delay_us(30);	//delay for command to be processed
	}
}