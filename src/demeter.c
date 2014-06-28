/*
 * main.c
 *
 *  Created on: 07/06/2014
 *      Author: sebas
 */

#include <modbus/include/mb.h>
#include <modbus/include/mbport.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <demeter.h>
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

#define	REG_INPUT_START	1

typedef enum {
	IR_TEMPERATURE, IR_HUMIDITY, IR_LIGHT, IR_NREGS
} input_registers;

FATFS fs;
FIL log_file;
DHT22_DATA_t sensor_values;
rtc_datetime_24h_t current_dt;
rtc_device_t *rtc = &rtc_ds1307;

volatile unsigned int ticks = 0;

static void timer0_callback(void);
static uint16_t get_seconds(instant_t t);
static inline void update_state(void);

#define JOIN_BYTES(u,l)		(u<<8) | (l & 0xFF)
/**
 * MCU: Atmega328
 * Fuses: Oscilador interno a 8 Mhz (sin dividir por 8)
 * 		-U lfuse:w:0xe2:m -U hfuse:w:0xd1:m -U efuse:w:0x07:m
 */
int main(void) {
	ports_init();

	adc_init();

	timer0_init(timer0_callback);

	i2c_init();

	rtc_init(rtc);
	rtc_sqw_rate(rtc, 1);
	rtc_sqw_enable(rtc);
	rtc_clock_start(rtc);

	eMBInit(MB_RTU, 0x03, 0, 9600, MB_PAR_NONE);
	eMBSetSlaveID(0x3, TRUE, (UCHAR*) "pepe", 4);
	eMBEnable();

	blinkenlight(5, 100);

	parameters_init();

	f_mount(&fs, "", 0);
	if (f_open(&log_file, LOG, FA_OPEN_ALWAYS | FA_WRITE) == FR_OK) {
		if (f_lseek(&log_file, f_size(&log_file)) == FR_OK) {
			LED_PORT |= _BV(LED);
			f_sync(&log_file);
		}
	}

	printf("\r\nAplicación inicializada!\r\n");
	while (1) {
		eMBPoll();
		update_state();
		_delay_ms(200);
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
	LED_PORT ^= _BV(LED);
}

void update_state(void) {
	DHT22_ERROR_t error = readDHT22(&sensor_values);
	rtc_read(rtc, &current_dt);
	event_t *events = get_events();

	uint16_t start_secs;
	uint16_t current_secs;
	uint8_t i = 0;

	current_secs = get_seconds((instant_t){current_dt.hour, current_dt.minute, current_dt.date});
	uint8_t flag = 0;
	for (; i < MAX_EVENTS; i++) {
		if (events[i].target != -1) {
			start_secs = get_seconds(events[i].start);
			if (start_secs < current_secs
					&& (start_secs + events[i].duration) > current_secs) {
				flag |= (1 << events[i].target);
			}
		}
	}
	for (i = 1; i <= 2; i++) {
		if (flag & (1 << i)) {
			enable_relay(i);
		} else {
			disable_relay(i);
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

static uint16_t get_seconds(instant_t t) {
	return (t.hour * 60 * 60) + (t.minute * 60) + t.second;
}

eMBErrorCode eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNRegs) {
	eMBErrorCode eStatus = MB_ENOERR;

	if ((usAddress >= REG_INPUT_START)
			&& (usAddress + usNRegs <= REG_INPUT_START + IR_NREGS)) {
		uint16_t ir = usAddress - REG_INPUT_START;
		int16_t value;
		while (usNRegs > 0) {
			switch (ir) {
			case IR_TEMPERATURE:
				value = sensor_values.raw_temperature;
				break;
			case IR_HUMIDITY:
				value = sensor_values.raw_humidity;
				break;
			case IR_LIGHT:
				value = adc_read(PHOTORESISTOR);
				break;
			default:
				value = -1	;
			}

			*pucRegBuffer++ = (value >> 8);
			*pucRegBuffer++ = (value & 0xFF);
			ir++;
			usNRegs--;
		}
	} else {
		eStatus = MB_ENOREG;
	}

	return eStatus;

	return MB_ENOREG;
}

eMBErrorCode eMBRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNRegs, eMBRegisterMode eMode) {
	eMBErrorCode status = MB_ENOERR;

	if (usAddress == 1 && usNRegs == 6) {
		// fecha
		if (eMode == MB_REG_READ) {
			*pucRegBuffer++ = current_dt.year >> 8;
			*pucRegBuffer++ = current_dt.year & 0xFF;

			*pucRegBuffer++ = current_dt.month >> 8;
			*pucRegBuffer++ = current_dt.month & 0xFF;

			*pucRegBuffer++ = current_dt.date >> 8;
			*pucRegBuffer++ = current_dt.date & 0xFF;

			*pucRegBuffer++ = current_dt.hour >> 8;
			*pucRegBuffer++ = current_dt.hour & 0xFF;

			*pucRegBuffer++ = current_dt.minute >> 8;
			*pucRegBuffer++ = current_dt.minute & 0xFF;

			*pucRegBuffer++ = current_dt.second >> 8;
			*pucRegBuffer++ = current_dt.second & 0xFF;
		} else {
			rtc_datetime_24h_t tmp;

			tmp.year = (*pucRegBuffer++ << 8);
			tmp.year |= (*pucRegBuffer++ & 0xFF);

			tmp.month = (*pucRegBuffer++ << 8);
			tmp.month |= (*pucRegBuffer++ & 0xFF);

			tmp.date = (*pucRegBuffer++ << 8);
			tmp.date |= (*pucRegBuffer++ & 0xFF);

			tmp.hour = (*pucRegBuffer++ << 8);
			tmp.hour |= (*pucRegBuffer++ & 0xFF);

			tmp.minute = (*pucRegBuffer++ << 8);
			tmp.minute |= (*pucRegBuffer++ & 0xFF);

			tmp.second = (*pucRegBuffer++ << 8);
			tmp.second |= (*pucRegBuffer++ & 0xFF);

			rtc_write(rtc, &tmp);
		}
	} else if (usAddress == 7 && usNRegs == 1) {
		// intervalo de logging
		if (eMode == MB_REG_READ) {
			//*pucRegBuffer++ = get_log_interval() >> 8;
			*pucRegBuffer++ = 0;
			*pucRegBuffer = get_log_interval();
		} else {
			set_log_interval(*++pucRegBuffer & 0xFF);
		}
	} else if (usAddress == 8 && usNRegs <= (MAX_EVENTS * sizeof(event_t)) && (usNRegs%sizeof(event_t) == 0)) {
		// intervalo de logging
		if (eMode == MB_REG_READ) {
			uint8_t count;
			event_t *events = get_events();

			for(count = 0; count < usNRegs / sizeof(event_t); count++) {
				event_t e = *events++;
				*pucRegBuffer++ = e.start.hour;
				*pucRegBuffer++ = e.start.minute;
				*pucRegBuffer++ = e.start.second;
				*pucRegBuffer++ = e.duration >> 8;
				*pucRegBuffer++ = e.duration & 0xFF;
				*pucRegBuffer++ = e.target;
			}
		} else {
		}
	} else {
		status = MB_ENOREG;
	}
	return status;
}

eMBErrorCode eMBRegCoilsCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNCoils, eMBRegisterMode eMode) {
	return MB_ENOREG;
}

eMBErrorCode eMBRegDiscreteCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNDiscrete) {
	return MB_ENOREG;
}
