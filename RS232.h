//***************************************************************************
//
// File Name            : RS232.h
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

#ifndef RS232_H_
#define RS232_H_

void init_RS232();
void send_char(char c);
char receive_char();
#endif /* RS232_H_ */