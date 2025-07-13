#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <StepCounter/step_counter.h>

volatile int16_t accelerometerBuffer[3];
volatile uint16_t stepCount;
volatile uint32_t maBuffer[MA_WINDOW_SIZE] = {0};
volatile uint8_t maIndex = 0;
bool bufferFull = false;


void accelerometer_init()
{
    stepCount = 0;
}

uint32_t movingAverage(uint32_t new_sample){
    maBuffer[maIndex] = new_sample;
    maIndex = (maIndex + 1) % MA_WINDOW_SIZE;
    if (maIndex == 0){
        bufferFull = true;
    }
    uint32_t sum = 0;
    uint8_t count;
    if(bufferFull){
        count = MA_WINDOW_SIZE;
    }else{
        count = maIndex;
    }
    uint8_t i;
    for(i = 0; i < count; i++){
        sum += maBuffer[i];
    }
    return (uint32_t)(sum / count);
}
