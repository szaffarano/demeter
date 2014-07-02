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

#define DATETIME_SIZE	(sizeof(datetime_t) / 2)
#define EVENTS_SIZE		((sizeof(event_t) * MAX_EVENTS) / 2)
#define LOGEVENT_SIZE	(sizeof(uint16_t) / 2)
#define STATE_SIZE		(sizeof(state_t) / 2)
#define NUMBER_OF_COILS	2

FATFS fs;
FIL log_file;
rtc_device_t *rtc = &rtc_ds1307;

state_t state;
datetime_t datetime;
uint8_t relays; /* estado de los relays, lo manejo con bitwise operations */

// "yyyymmdd.log"
static char log_filename[13] = "nn.log";
static char last_logfilename[13] = "empty.log";

volatile unsigned int ticks = 0;

static void timer0_callback(void);
static inline long get_seconds(instant_t t);
static inline void update_state(void);
static inline void update_log_filename(void);
/**
 * MCU: Atmega328
 * Fuses: Oscilador interno a 8 Mhz (sin dividir por 8)
 * 		-U lfuse:w:0xe2:m -U hfuse:w:0xd1:m -U efuse:w:0x07:m
 */
int main(void) {
	adc_init();

	timer0_init(timer0_callback);

	i2c_init();

	rtc_init(rtc);
	rtc_sqw_rate(rtc, 1);
	rtc_sqw_enable(rtc);
	rtc_clock_start(rtc);

	eMBInit(MB_RTU, 0x03, 0, 9600, MB_PAR_NONE);
	eMBSetSlaveID(0x3, TRUE, (UCHAR*) "demeter", 8);
	eMBEnable();

	blinkenlight(5, 100);

	parameters_init();

	ports_init();

	f_mount(&fs, "", 0);
	update_log_filename();

	while (1) {
		eMBPoll();
		update_state();

		_delay_ms(100);
	}

	return (0);
}

static inline void update_state(void) {
	DHT22_DATA_t sensor_values;
	rtc_datetime_24h_t current_dt;
	readDHT22(&sensor_values);
	rtc_read(rtc, &current_dt);

	event_t* events = get_events();
	long start_secs;
	long current_secs;
	uint8_t i = 0;
	uint8_t flag = 0;

	state.temperature = sensor_values.raw_temperature;
	state.humidity = sensor_values.raw_humidity;
	state.light = adc_read(PHOTORESISTOR);

	datetime = (datetime_t ) { .year = current_dt.year, .month =
					current_dt.month, .date = current_dt.date, .hour =
					current_dt.hour, .minute = current_dt.minute, .second =
					current_dt.second };

	current_secs = get_seconds((instant_t ) {
					datetime.hour,
					datetime.minute,
					datetime.second });

	// recorro eventos para ver si tengo que activar relays
	for (i = 0; i < MAX_EVENTS; i++) {
		if (events[i].enabled) {
			start_secs = get_seconds(events[i].start);
			if (start_secs < current_secs
					&& (start_secs + events[i].duration) > current_secs) {
				flag |= (1 << events[i].target); /* activar el evento */
			}
		}
	}

	// si activaron coil, estamos dentro de un evento o presionaron pulsador
	for (uint8_t i = 0; i < NUMBER_OF_COILS; i++) {
		if (relays & (1 << i) || (flag & (1 << i)) || is_pushed()) {
			enable_relay(i);
		} else {
			disable_relay(i);
		}
	}

	if (ticks >= get_log_interval()) {
		update_log_filename(); /* si cambio el dia y hay que hacer roll del archivo */

		//yyyyMMddhhmmss	light	humidity	temperature
		f_printf(&log_file, "%04d%02d%02d%02d%02d%02d\t%d\t%d\t%d\n",
				datetime.year, datetime.month, datetime.date, datetime.hour,
				datetime.minute, datetime.second, state.light, state.humidity,
				state.temperature);
		f_sync(&log_file);
		ticks = 0;
	}
}

void timer0_callback() {
	ticks++;
	LED_PORT ^= _BV(LED);
}

/* Callbacks Modbus */

eMBErrorCode eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNRegs) {

	if ((usAddress + usNRegs - 1) > STATE_SIZE) {
		return MB_ENOREG;
	}
	int16_t* raw_state = (int16_t*) &state;

	raw_state += usAddress - 1;
	while (usNRegs > 0) {
		*pucRegBuffer++ = (*raw_state >> 8);
		*pucRegBuffer++ = (*raw_state & 0xFF);
		usNRegs--;
		raw_state++;
	}

	return MB_ENOERR;
}

/**
 * Holding registers (empieza en 1):
 * 	LOG_INTERVAL + DATETIME + EVENTS
 */
eMBErrorCode eMBRegHoldingCB(UCHAR * buffer, USHORT address, USHORT regs,
		eMBRegisterMode mode) {

	// indice para saber por qué grupo de registros voy avanzando
	uint16_t idx = address;

	// hago operaciones de bit en una variable, para no usar dos
	// es para indicar si debo actualizar el grupo de registros
	// de eventos y/o datetime.
	uint8_t flag = 0;

	// grupo de registros correspondiente a datetime
	uint16_t* dt = (uint16_t*) &datetime;

	// grupo de registros correspondiente a los eventos.
	uint16_t* events = (uint16_t*) get_events();

	if ((address + regs - 1) > (EVENTS_SIZE + LOGEVENT_SIZE + DATETIME_SIZE)) {
		return MB_ENOREG;
	}

	// si me piden algo que va mas allá de logevent (primer registro)
	// avanzo el siguiente grupo, datetime
	if (address > LOGEVENT_SIZE) {
		dt += address - LOGEVENT_SIZE - 1;
	}

	// si me piden algo que va mas allá del segundo registro (datetime)
	// avanzo el tercer grupo, events hasta donde haga falta.
	if (address > (LOGEVENT_SIZE + DATETIME_SIZE)) {
		events += address - LOGEVENT_SIZE - DATETIME_SIZE - 1;
	}

	// operación de lectura
	if (mode == MB_REG_READ) {
		for (; idx < regs + address; idx++) {
			if (idx > 0 && idx <= LOGEVENT_SIZE) {
				uint16_t log_interval = get_log_interval();
				*buffer++ = (log_interval >> 8);
				*buffer++ = (log_interval & 0xFF);
			} else if (idx > LOGEVENT_SIZE
					&& idx <= (LOGEVENT_SIZE + DATETIME_SIZE)) {
				*buffer++ = (*dt >> 8);
				*buffer++ = (*dt & 0xFF);
				dt++;
			} else {
				*buffer++ = (*events >> 8);
				*buffer++ = (*events & 0xFF);
				events++;
			}
		}
		// operación de escritura
	} else {
		for (; idx < regs + address; idx++) {
			if (idx > 0 && idx <= LOGEVENT_SIZE) {
				uint16_t log_interval = (*buffer++ << 8);
				log_interval |= (*buffer++ & 0xFF);
				set_log_interval(log_interval);
			} else if (idx > LOGEVENT_SIZE
					&& idx <= (LOGEVENT_SIZE + DATETIME_SIZE)) {
				*dt = (*buffer++ << 8);
				*dt |= (*buffer++ & 0xFF);
				dt++;
				flag |= (1 << 1); /* se modifica algun campo de la fecha, marco flag */
			} else {
				*events = (*buffer++ << 8);
				*events |= (*buffer++ & 0xFF);
				events++;
				flag |= (1 << 2); /* se modifica algún evento, marco flag */
			}
		}

		if ((flag & (1 << 1))) {
			rtc_datetime_24h_t newdate;
			newdate.year = datetime.year;
			newdate.month = datetime.month;
			newdate.date = datetime.date;
			newdate.hour = datetime.hour;
			newdate.minute = datetime.minute;
			newdate.second = datetime.second;
			rtc_write(rtc, &newdate);
		}

		if ((flag & (1 << 2))) {
			set_events(get_events());
		}
	}

	return MB_ENOERR;
}

eMBErrorCode eMBRegCoilsCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNCoils, eMBRegisterMode eMode) {
	if (usAddress + usNCoils - 1 > NUMBER_OF_COILS) {
		return MB_ENOREG;
	}
	usAddress--; /* empiezo en cero. */
	if (eMode == MB_REG_READ) {
		while (usNCoils > 0) {
			if (relays & (1 << usAddress)) {
				*pucRegBuffer |= (1 << usAddress);
			}
			usNCoils--;
			usAddress++;
		}
	} else {
		*pucRegBuffer <<= usAddress;
		while (usNCoils > 0) {
			if ((*pucRegBuffer & (1 << usAddress))) {
				relays |= (1 << usAddress);
			} else {
				relays &= ~(1 << usAddress);
			}
			usNCoils--;
			usAddress++;
		}
	}
	return MB_ENOERR;
}

eMBErrorCode eMBRegDiscreteCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNDiscrete) {
	return MB_ENOREG;
}

/**
 * Función usada para setearle la fecha a los archivos.
 */
DWORD get_fattime(void) {
	rtc_datetime_24h_t current_dt;
	rtc_read(rtc, &current_dt);
	return ((DWORD) (current_dt.year - 1980) << 25) /* Year */
	| ((DWORD) current_dt.month << 21) /* Month */
	| ((DWORD) current_dt.date << 16) /* Mday */
	| ((DWORD) current_dt.hour << 11) /* Hour */
	| ((DWORD) current_dt.minute << 5) /* Min */
	| ((DWORD) current_dt.second >> 1); /* Sec */
}

/**
 * Convierte un instant_t a segundos.  Esos segundos representan el tiempo
 * transcurrido desde las 0 hs hasta el momento del instante
 * propiamente dicho.
 */
static inline long get_seconds(instant_t t) {
	return (t.hour * 60 * 60) + (t.minute * 60) + t.second;
}

static inline void update_log_filename(void) {
	rtc_datetime_24h_t dt;
	rtc_read(rtc, &dt);
	sprintf(log_filename, "%04d%02d%02d.log", dt.year, dt.month, dt.date);
	if (strcmp(log_filename, last_logfilename) != 0) {
		f_close(&log_file);
		if (f_open(&log_file, log_filename, FA_OPEN_ALWAYS | FA_WRITE)
				== FR_OK) {
			if (f_lseek(&log_file, f_size(&log_file)) == FR_OK) {
				f_sync(&log_file);
			}
		}
		strcpy(last_logfilename, log_filename);
	}
}
