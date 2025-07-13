#include <Controls/joystick.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/inc/msp.h>

volatile uint16_t joystickCoordBuffer[2];
volatile uint8_t joystickDataReady = 0;

uint8_t joystickDirection = 0;

#define JOY_LEFT_THRESHOLD_LOW  0
#define JOY_LEFT_THRESHOLD_HIGH 2000
#define JOY_RIGHT_THRESHOLD_LOW 14000
#define JOY_RIGHT_THRESHOLD_HIGH 16383

#define JOY_DOWN_THRESHOLD_LOW 0
#define JOY_DOWN_THRESHOLD_HIGH 2000
#define JOY_UP_THRESHOLD_LOW 14000
#define JOY_UP_THRESHOLD_HIGH 16383


uint8_t decodeJoystickDirection(uint16_t x, uint16_t y) {
    // UP
    if ((y >= JOY_UP_THRESHOLD_LOW && y <= JOY_UP_THRESHOLD_HIGH) && (x >= JOY_LEFT_THRESHOLD_HIGH && x <= JOY_RIGHT_THRESHOLD_LOW)) {
        return 1;
    }
    // RIGHT
    else if ((x >= JOY_RIGHT_THRESHOLD_LOW && x <= JOY_RIGHT_THRESHOLD_HIGH) && (y >= JOY_DOWN_THRESHOLD_HIGH && y <= JOY_UP_THRESHOLD_LOW)) {
        return 2;
    }
    // DOWN
    else if ((y <= JOY_DOWN_THRESHOLD_HIGH) && (x >= JOY_LEFT_THRESHOLD_HIGH && x <= JOY_RIGHT_THRESHOLD_LOW)) {
        return 3;
    }
    // LEFT
    else if ((x <= JOY_LEFT_THRESHOLD_HIGH) && (y >= JOY_DOWN_THRESHOLD_HIGH && y <= JOY_UP_THRESHOLD_LOW)) {
        return 4;
    }
    return 0; // Default neutral if no condition matched
}

