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
#include <stdlib.h>

#include <sandbox.h>
#include <adc/adc.h>
#include <ports/ports.h>
#include <timer/timer.h>

#include <config/config.h>

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

volatile unsigned int ticks = 0;

static void timer0_callback(void);
static void blinkenlight(uint8_t times, uint8_t delay);
static uint8_t get_number(const char* msg);
static uint16_t get_seconds(rtc_datetime_24h_t t);

/**
 * MCU: Atmega328
 * Fuses: Oscilador interno a 8 Mhz (sin dividir por 8)
 * 		lfuse: E2
 * 		hfuse: D9 (D1 para no borrar la eeprom al flashear)
 * 		efuse: 7
 */
int main(void) {
	uart_init(UART_BAUD_RATE);

	ports_init();
	adc_init();
	timer1_init(timer0_callback);

	i2c_init();

	rtc_init(rtc);
	rtc_sqw_rate(rtc, 1);
	rtc_sqw_enable(rtc);
	rtc_clock_start(rtc);

	sei();

	blinkenlight(5, 100);

	printf("Inicializando.");

	parameters_init();

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
	uint8_t i = 0;
	event_t* events = get_events();
	while (1) {
		unsigned char r = uart_getc();
		switch (r) {
		case 's':
			printf("Configurar [I]ntervalo de log o [E]vento: ");
			do {
				r = uart_getc();
			} while (r == 0);
			if (r == 'I') {
				i = get_number("\r\nIngrese un valor numérico: ");
				printf("seteando intervalo en %d\r\n", i);
				set_log_interval(i);
			} else if (r == 'E') {
				printf("\r\n");
				i = get_number("Ingrese numero de evento a modificar: ");
				events[i].start.hour = get_number(
						"Ingrese la hora de inicio: ");
				events[i].start.minute = get_number(
						"Ingrese los minutos de inicio: ");
				events[i].start.second = get_number(
						"Ingrese los segundos de inicio: ");
				events[i].duration = get_number(
						"Ingrese la duración del evento: ");
				events[i].target = get_number(
						"Ingrese el número de relay (0/1): ");
				printf(
						"Grabando evento %02d:%02d:%02d con duración %5d  segundos en relay #%d\r\n",
						events[i].start.hour, events[i].start.minute,
						events[i].start.second, events[i].duration,
						events[i].target);
				set_events(events);
			} else {
				printf("Opción errónea\r\n");
			}
			break;
		case 'c':
			printf(
					"%02d/%02d/%04d %02d:%02d:%02d: Luz=%d Humedad=%d %% Temperatura= %d *C (DHT status: %d) interval: %d\r\n",
					current_dt.date, current_dt.month, current_dt.year,
					current_dt.hour, current_dt.minute, current_dt.second,
					adc_read(PHOTORESISTOR), sensor_values.raw_humidity,
					sensor_values.raw_temperature, error, get_log_interval());
			printf("Eventos:\r\n");
			for (i = 0; i < MAX_EVENTS; i++) {
				printf(
						"\tGrabando evento %02d:%02d:%02d con duración %5d segundos en relay #%d\r\n",
						events[i].start.hour, events[i].start.minute,
						events[i].start.second, events[i].duration,
						events[i].target);

			}
			break;
		case 'r':
			i = get_number("Ingrese relay a activar: ");
			enable_relay(i);
			break;
		case 'n':
			i = get_number("Ingrese relay a desactivar: ");
			disable_relay(i);
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
	return ((DWORD) (current_dt.year - 1980) << 25) /* Year */
	| ((DWORD) current_dt.month << 21) /* Month */
	| ((DWORD) current_dt.date << 16) /* Mday */
	| ((DWORD) current_dt.hour << 11) /* Hour */
	| ((DWORD) current_dt.minute << 5) /* Min */
	| ((DWORD) current_dt.second >> 1); /* Sec */
}

void timer0_callback() {
	ticks++;
	error = readDHT22(&sensor_values);
	rtc_read(rtc, &current_dt);
	event_t *events = get_events();

	uint16_t start_secs;
	uint16_t current_secs;
	uint8_t i = 0;

	current_secs = get_seconds(current_dt);
	for (; i < MAX_EVENTS; i++) {
		if (events[i].target != -1) {
			start_secs = get_seconds(events[i].start);
			if (start_secs < current_secs
					&& (start_secs + events[i].duration) > current_secs) {
				enable_relay(events[i].target);
			} else {
				disable_relay(events[i].target);
			}
		}
	}

	if (ticks >= get_log_interval()) {
		f_printf(&log_file,
				"%02d/%02d/%04d %02d:%02d:%02d: Luz=%d Humedad=%d %% Temperatura= %d *C (DHT status: %d)\n",
				current_dt.date, current_dt.month, current_dt.year,
				current_dt.hour, current_dt.minute, current_dt.second,
				adc_read(PHOTORESISTOR), sensor_values.raw_humidity,
				sensor_values.raw_temperature, error);
		f_sync(&log_file);
		ticks = 0;
	}
}

static void blinkenlight(uint8_t times, uint8_t delay) {
	int i = 0;
	for (i = 0; i < times; i++) {
		LED_PORT ^= _BV(LED);
		_delay_ms(delay / 2);
		LED_PORT ^= _BV(LED);
		_delay_ms(delay);
	}
	LED_PORT &= ~_BV(LED);
}

static uint8_t get_number(const char* msg) {
	char buffer[4]; // maximo 255
	uint8_t r = 0, i = 0;
	printf(msg);
	do {
		r = uart_getc();
		if (r != 0 && r != '\n') {
			if ((r - '0' >= 0 && (r - '0') <= 9)) {
				buffer[i++] = r;
				printf("%c", r);
			}
		}
	} while (r != '\r' && i < 4);
	printf("\r\n");
	buffer[i] = '\0';
	return atoi(buffer);
}

static uint16_t get_seconds(rtc_datetime_24h_t t) {
	return (t.hour * 60 * 60) + (t.minute * 60) + t.second;
}
