#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void temperature_init(void);
double convert_temperature(double f);

#endif /* TEMPERATURE_H_ */
