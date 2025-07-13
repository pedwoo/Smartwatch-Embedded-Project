// rtc.h
#ifndef CLOCK_H
#define CLOCK_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

extern volatile uint8_t data_seconds;
extern volatile uint8_t data_minutes;
extern volatile uint8_t data_hours;
extern volatile uint8_t data_dayOfWeek;
extern volatile uint8_t data_dayOfMonth;
extern volatile uint8_t data_month;
extern volatile uint16_t data_year;

// Initializes the RTC and starts the clock with a default date/time
void date_init(void);

#endif // CLOCK_H
