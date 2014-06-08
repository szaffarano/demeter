/*
 * rtc.c
 *
 *  Created on: Dec 24, 2012
 *      Author: jcole
 */

#include "rtc.h"

uint8_t rtc_days_in_month[13][2] = {
		{ 0, 0 },
		{ 31, 31 },
		{ 28, 29 },
		{ 31, 31 },
		{ 30, 30 },
		{ 31, 31 },
		{ 30, 30 },
		{ 31, 31 },
		{ 31, 31 },
		{ 30, 30 },
		{ 31, 31 },
		{ 30, 30 },
		{ 31, 31 } };

char *rtc_month_abbreviations[13] = {
		"",
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec" };

char *rtc_dow_names[8] = {
		"",
		"Sunday",
		"Monday",
		"Tuesday",
		"Wednesday",
		"Thursday",
		"Friday",
		"Saturday" };

uint8_t rtc_init(rtc_device_t *rtc) {
	return ((*rtc->init)());
}

uint8_t rtc_clock_start(rtc_device_t *rtc) {
	return ((*rtc->clock_start)());
}

uint8_t rtc_clock_stop(rtc_device_t *rtc) {
	return ((*rtc->clock_stop)());
}

uint8_t rtc_sqw_enable(rtc_device_t *rtc) {
	return ((*rtc->sqw_enable)());
}

uint8_t rtc_sqw_disable(rtc_device_t *rtc) {
	return ((*rtc->sqw_disable)());
}

uint8_t rtc_sqw_rate(rtc_device_t *rtc, uint16_t rate) {
	return ((*rtc->sqw_rate)(rate));
}

uint8_t rtc_read(rtc_device_t *rtc, rtc_datetime_24h_t *dt) {
	return ((*rtc->read)(dt));
}

uint8_t rtc_write(rtc_device_t *rtc, rtc_datetime_24h_t *dt) {
	return ((*rtc->write)(dt));
}

uint8_t rtc_find_dow(uint16_t y, uint8_t m, uint8_t d) {
	static uint8_t t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	y -= m < 3;
	return (uint8_t) (1 + ((y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7));
}

int8_t rtc_find_dst_offset(rtc_datetime_24h_t time, rtc_dst_date_t dst_dates[]) {
	uint8_t i;

	for (i = 0; dst_dates[i].year != 0; i++) {
		/* Skip through the dst_dates array looking for the current year. */
		if (dst_dates[i].year > time.year)
			return (0);
		if (dst_dates[i].year < time.year)
			continue;

		/* The year matches, check the month, day, and hour to try to
		 * discern whether DST is in effect. */
		if ((time.month == dst_dates[i].start_month)
				&& (time.date == dst_dates[i].start_date) && (time.hour >= 2))
			return (1);
		if ((time.month == dst_dates[i].end_month)
				&& (time.date == dst_dates[i].end_date) && (time.hour < 2))
			return (1);
		if ((time.month == dst_dates[i].start_month)
				&& (time.date > dst_dates[i].start_date))
			return (1);
		if ((time.month == dst_dates[i].end_month)
				&& (time.date < dst_dates[i].end_date))
			return (1);
		if ((time.month > dst_dates[i].start_month)
				&& (time.month < dst_dates[i].end_month))
			return (1);

		/* None of the above were true; DST is not in effect currently. */
		return (0);
	}

	/* We didn't find an entry in the dst_dates list; assume no DST. */
	return (0);
}

uint8_t rtc_offset_time(rtc_datetime_24h_t *from, rtc_datetime_24h_t *to,
		uint8_t offset_hours) {
	to->second = from->second;
	to->minute = from->minute;
	to->hour = from->hour;
	to->date = from->date;
	to->month = from->month;
	to->year = from->year;
	to->day_of_week = from->day_of_week;

	to->hour += offset_hours;

	if (to->hour < 0) {
		to->day_of_week -= 1;
		if (to->day_of_week == 0) {
			to->day_of_week = 7;
		}

		to->date -= 1;
		if (to->date == 0) {
			to->month -= 1;
			if (to->month == 0) {
				to->year -= 1;
				to->month = 12;
			}
			to->date = rtc_days_in_month[to->month][0]; /* XXX */
		}
		to->hour += 24;
	}

	if (to->hour >= 24) {
		to->day_of_week += 1;
		if (to->day_of_week == 8) {
			to->day_of_week = 1;
		}

		to->date += 1;
		if (to->date > rtc_days_in_month[to->month][0]) {
			to->month += 1;
			if (to->month == 13) {
				to->year += 1;
				to->month = 1;
			}
			to->date = 1;
		}
		to->hour -= 24;
	}

	return 0;
}

static uint8_t conv2d(const char* p) {
	uint8_t v = 0;
	if ('0' <= *p && *p <= '9')
		v = *p - '0';
	return 10 * v + *++p - '0';
}

void set_compile_dt(const char* date, const char* time, rtc_datetime_24h_t* dt) {
	// sample input: date = "Dec 26 2009", time = "12:34:56"
	dt->year = 2000 + conv2d(date + 9);
	// Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
	switch (date[0]) {
	case 'J':
		if (date[1] == 'a') {
			dt->month = 1;
		} else if (date[2] == 'n') {
			dt->month = 6;
		} else {
			dt->month = 7;
		}
		break;
	case 'F':
		dt->month = 2;
		break;
	case 'A':
		dt->month = date[2] == 'r' ? 4 : 8;
		break;
	case 'M':
		dt->month = date[2] == 'r' ? 3 : 5;
		break;
	case 'S':
		dt->month = 9;
		break;
	case 'O':
		dt->month = 10;
		break;
	case 'N':
		dt->month = 11;
		break;
	case 'D':
		dt->month = 12;
		break;
	}
	dt->date = conv2d(date + 4);
	dt->hour = conv2d(time);
	dt->minute = conv2d(time + 3);
	dt->second = conv2d(time + 6) + 13;
}

