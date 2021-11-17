//***************************************************************************
//
// File Name            : LCD_DOG.h
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



#ifndef LCD_DOG_H_
#define LCD_DOG_H_

void init_spi_lcd();
void lcd_spi_transmit_CMD(unsigned char cmd);
void lcd_spi_transmit_DATA(unsigned char cmd);
void init_lcd_dog();
void update_lcd_dog();

//create display buffer for each line
char display_buff1[17];
char display_buff2[17];
char display_buff3[17];

#endif /* LCD_DOG_H_ */