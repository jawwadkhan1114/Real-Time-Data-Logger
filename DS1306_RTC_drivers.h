/*
 * DS1306_RTC_drivers.h
 *
 * Created: 4/20/2021 5:02:58 PM
 *  Author: jawwa
 */ 


#ifndef DS1306_RTC_DRIVERS_H_
#define DS1306_RTC_DRIVERS_H_
#include <avr/io.h>

void SPI_rtc_ds1306_config();
void write_RTC (unsigned char reg_RTC, unsigned char data_RTC);
unsigned char read_RTC (unsigned char reg_RTC);
void write_read_RTC_test();
void block_read_RTC(volatile unsigned char *array_ptr, unsigned char strt_addr, unsigned char count);
void block_write_RTC (volatile unsigned char *array_ptr, unsigned char strt_addr, unsigned char count);
void block_write_read_RTC_test();
void init_clock();
void poll_clock();

uint8_t rtc_byte[10];
volatile unsigned char RTC_time_date_write[7];
volatile unsigned char RTC_time_date_read[7];
volatile uint8_t converted_time_values[6];

#endif /* DS1306_RTC_DRIVERS_H_ */