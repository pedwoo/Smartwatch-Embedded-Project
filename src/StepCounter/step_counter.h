#ifndef STEP_COUNTER_H_
#define STEP_COUNTER_H_

#include <stdint.h>


#define MA_WINDOW_SIZE 100

extern volatile int16_t accelerometerBuffer[3];
extern volatile uint16_t stepCount;
extern volatile uint32_t maBuffer[MA_WINDOW_SIZE];
extern volatile uint8_t maIndex;
extern bool bufferFull;

// Initializes the accelerometer and ADC for step counting
void accelerometer_init(void);
uint32_t movingAverage(uint32_t new_sample);

#endif /* STEP_COUNTER_H_ */

