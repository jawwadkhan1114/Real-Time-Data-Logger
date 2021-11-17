//***************************************************************************
//
// File Name            : RS232.c
// Title                :
// Date                 : 4/14/2021
// Version              : 1.0
// Target MCU           : AVR128DB48
// Target Hardware      ;
// Author               : Jawwad Khan
// DESCRIPTION
// Initialize USART3 of microcontroller.
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

#define F_CPU 4000000
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "ADC.h"
#include "LCD_DOG.h"
#include "RS232.h"

#define numdatabit 0x03
#define noparity 0x00 << 4
#define onestop 0x00 << 3
#define pb1_bm 0x01
#define f_baud_low 0b10000010
#define f_baud_high 0b00000110


//***************************************************************************
//
// Function Name        : init_RS232
// Date                 : 4/14/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Jawwad Khan
// DESCRIPTION
// Function to internal USART.
//
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void init_RS232(){
	
	uint8_t high_b = f_baud_high;
	uint8_t low_b = f_baud_low;
	
	
	//set pb1 as output
	PORTB.DIR = pb1_bm;
	
	//enable receiver and transfer
	USART3.CTRLB = (USART_RXEN_bm | USART_TXEN_bm);
	//set no parity, 1 stop bit, 8 data
	USART3.CTRLC = numdatabit | noparity | onestop;
	//set baud rate
	USART3.BAUDL = low_b;
	USART3.BAUDH = high_b;
	
	//enable interupts
	//interrupt triggers when there is unread data in rx register
	USART3.CTRLA = USART_RXCIF_bm;
}
//***************************************************************************
//
// Function Name        : send_char
// Date                 : 4/14/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Jawwad Khan
// DESCRIPTION
// Function to send out character to tx.
//
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void send_char(char c){
	USART3.TXDATAL = c;
	//wait for data register to become empty
	while((USART3.STATUS & USART_DREIF_bm) != USART_DREIF_bm){ }
	USART3.STATUS |= USART_TXCIF_bm;
}

char receive_char(){
	char temp;
	temp = USART3.RXDATAL;
	while((USART3.STATUS & USART_RXCIF_bm) == USART_RXCIF_bm){}
	return temp;
}