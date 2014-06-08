/*
 * main.c
 *
 *  Created on: 07/06/2014
 *      Author: sebas
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <uart/uart.h>
#include <string.h>

#include <sandbox.h>
#include <adc/adc.h>
#include <ports/ports.h>
#include <timer/timer.h>

#include <dht/DHT22.h>

#include <stdio.h>

#include <ff/ff.h>

#include <i2c/i2c.h>
#include <rtc/rtc.h>
#include <rtc/rtc_ds1307.h>

FATFS fs;
FIL log_file;
DHT22_DATA_t sensor_values;
DHT22_ERROR_t error;
rtc_datetime_24h_t current_dt;
rtc_device_t *rtc = &rtc_ds1307;

volatile unsigned int tics = 0;

static void cb();
static void welcome_blinks(uint8_t times, uint8_t delay);
/**
 * MCU: Atmega328
 * Fuses: Oscilador interno a 8 Mhz (sin dividir por 8)
 * 		lfuse: E2
 * 		hfuse: D9
 * 		efuse: 7
 */
int main(void) {
	uart_init(UART_BAUD_RATE);

	ports_init();
	adc_init();
	timer1_init(cb);

	i2c_init();

	rtc_init(rtc);
	rtc_sqw_rate(rtc, 1);
	rtc_sqw_enable(rtc);
	rtc_clock_start(rtc);

	sei();

	welcome_blinks(5, 100);

	printf("Inicializando.");
	f_mount(&fs, "", 0);
	printf(".");
	if (f_open(&log_file, LOG, FA_OPEN_ALWAYS | FA_WRITE) == FR_OK) {
		printf(".");
		if (f_lseek(&log_file, f_size(&log_file)) == FR_OK) {
			printf(".");
			LED_PORT |= _BV(LED);
		} else {
			f_printf(&log_file, "fallo el seek");
			f_sync(&log_file);
			printf("x");
		}
	} else {
		printf("x");
	}

	printf("\r\nAplicación inicializada!\r\n");
	while (1) {
		unsigned char r = uart_getc();
		switch (r) {
		case 's':
			printf(
					"%02d/%02d/%04d %02d:%02d:%02d: Luz=%d Humedad=%d %% Temperatura= %d *C (DHT status: %d)\r\n",
					current_dt.date, current_dt.month, current_dt.year,
					current_dt.hour, current_dt.minute, current_dt.second,
					adc_read(PHOTORESISTOR), sensor_values.raw_humidity,
					sensor_values.raw_temperature, error);
			break;
		case 'b':
			printf("%02i/%02i/%04i %02i:%02i:%02i Letra B\r\n", current_dt.date,
					current_dt.month, current_dt.year, current_dt.hour,
					current_dt.minute, current_dt.second);
			break;
		case 0:
			break;
		default:
			printf("Opción desconocida: %c\r\n", r);
		}
	}

	return (0);
}

DWORD get_fattime(void) {
	return ((DWORD) current_dt.year << 25) /* Year */
	| ((DWORD) current_dt.month << 21) /* Month */
	| ((DWORD) current_dt.date << 16) /* Mday */
	| ((DWORD) current_dt.hour << 11) /* Hour */
	| ((DWORD) current_dt.minute << 5) /* Min */
	| ((DWORD) current_dt.second >> 1); /* Sec */
}

void cb(int count) {
	tics++;
	error = readDHT22(&sensor_values);
	rtc_read(rtc, &current_dt);

	if (tics == LOG_INTERVAL) {
		f_printf(&log_file,
				"%02d/%02d/%04d %02d:%02d:%02d: Luz=%d Humedad=%d %% Temperatura= %d *C (DHT status: %d)\n",
				current_dt.date, current_dt.month, current_dt.year,
				current_dt.hour, current_dt.minute, current_dt.second,
				adc_read(PHOTORESISTOR), sensor_values.raw_humidity,
				sensor_values.raw_temperature, error);
		f_sync(&log_file);
		tics = 0;
	}
}

static void welcome_blinks(uint8_t times, uint8_t delay) {
	int i = 0;
	for (i = 0; i < times; i++) {
		LED_PORT ^= _BV(LED);
		_delay_ms(delay / 2);
		LED_PORT ^= _BV(LED);
		_delay_ms(delay);
	}
	LED_PORT &= ~_BV(LED);
}
