#include <Controls/adc_converter.h>
#include <Controls/buttons.h>
#include <Controls/joystick.h>
#include <LcdDriver/LcdDriver.h>
#include <StepCounter/step_counter.h>
#include <StepCounter/step_timer.h>
#include <Stopwatch/stopwatch.h>
#include <TemperatureSensor/HAL_TMP006.h>
#include <TemperatureSensor/temperature.h>
#include <WallClock/clock.h>
#include <lcdDriver/deepsleep_print.h>
#include <sensors/joystick.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/grlib/grlib.h>

void handleTimerState(void);

typedef enum {
    STATE_DEEPSLEEP,
    STATE_IDLE,
    STATE_STOPWATCH,
    STATE_STEP_COUNT,
    STATE_TEMPERATURE,
    STATE_TIMER

} SystemState;

typedef enum { CELSIUS, FARENHEIT } SystemTemperatureFormat;

typedef enum { ACTIVE, STOP, INIT, END } SystemTimerState;

typedef enum { SEC, MIN } SystemTimerSet;

SystemState currentState = STATE_DEEPSLEEP;
SystemTemperatureFormat currentTemperatureFormat = CELSIUS;
SystemTimerState currentTimerState = INIT;
SystemTimerSet currentTimerSet = SEC;

Graphics_Context g_sContext;

void _hwInit() {
    adc_init();  // Initialize ADC14 converter for all sensors

    buttons_init();      // Initialize buttons to check when pressed
    stopwatch_init();    // Initialize stopwatch
    date_init();         // Initialize date and time (manually)
    temperature_init();  // Initialize the temperature sensor

    graphics_init();  // Initialize graphics drivers

    // Initialize accelerometer and step timer
    step_timer_init();
    accelerometer_init();

    WDT_A_holdTimer();

    /* Initializes Clock System */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_16);
}

void handleStateChange(uint8_t joystickDirection) {
    int changeState = 0;
    switch (currentState) {
        case STATE_IDLE: {
            if (button_s1_pressed) {
                currentState = STATE_DEEPSLEEP;
                changeState = 1;
            } else {
                if (joystickDirection == 2) {
                    currentState = STATE_STOPWATCH;
                    changeState = 1;
                } else if (joystickDirection == 4) {
                    currentState = STATE_STEP_COUNT;
                    changeState = 1;
                }
            }
            break;
        }
        case STATE_STOPWATCH: {
            if (!stopwatchActive) {
                if (button_s1_pressed) {
                    currentState = STATE_DEEPSLEEP;
                    stopwatchCount = 0;
                    changeState = 1;
                } else {
                    if (joystickDirection == 2) {
                        currentState = STATE_TIMER;
                        currentTimerState = INIT;
                        changeState = 1;
                    } else if (joystickDirection == 4) {
                        currentState = STATE_IDLE;
                        stopwatchCount = 0;
                        changeState = 1;
                    }
                }
            }
            break;
        }
        case STATE_TIMER: {
            if (!stopwatchActive) {
                if (joystickDirection == 2) {
                    currentState = STATE_TEMPERATURE;
                    stopwatchCount = 0;
                    changeState = 1;
                } else if (joystickDirection == 4) {
                    currentState = STATE_STOPWATCH;
                    stopwatchCount = 0;
                    changeState = 1;
                }
            }

            break;
        }
        case STATE_STEP_COUNT: {
            if (button_s1_pressed) {
                currentState = STATE_DEEPSLEEP;
                changeState = 1;
            } else {
                if (joystickDirection == 2) {
                    currentState = STATE_IDLE;
                    changeState = 1;
                } else if (joystickDirection == 4) {
                    currentState = STATE_TEMPERATURE;
                    changeState = 1;
                }
            }
            break;
        }
        case STATE_TEMPERATURE: {
            if (button_s1_pressed) {
                currentState = STATE_DEEPSLEEP;
                changeState = 1;
            } else {
                if (joystickDirection == 2) {
                    currentState = STATE_STEP_COUNT;
                    changeState = 1;
                } else if (joystickDirection == 4) {
                    currentState = STATE_TIMER;
                    currentTimerState = INIT;
                    changeState = 1;
                }
            }
            break;
        }
        case STATE_DEEPSLEEP: {
            if (button_s2_pressed) {
                currentState = STATE_IDLE;
                changeState = 1;
            }
            break;
        }
    }
    if (changeState == 1) clear_lcd(g_sContext);
}

void handleTimerStateChange() {
    switch (currentTimerState) {
        case INIT: {
            if (button_s2_pressed) currentTimerState = ACTIVE;
            break;
        }
        case ACTIVE: {
            __delay_cycles(1500000);
            if (button_s2_pressed) currentTimerState = STOP;
            if (stopwatchCount >= totalTicks) currentTimerState = END;
            break;
        }
        case STOP: {
            __delay_cycles(1500000);
            if (button_s2_pressed) currentTimerState = ACTIVE;
            break;
        }
        case END: {
            __delay_cycles(18000000);
            clear_lcd(g_sContext);
            currentTimerState = INIT;
            break;
        }
    }
}

void handleSystemState() {
    switch (currentState) {
        case STATE_DEEPSLEEP: {
            deepsleep_print(g_sContext);
            PCM_gotoLPM0();
            break;
        }
        case STATE_IDLE: {
            // Display Date and time on the LCD
            const char *weekDays[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
            const char *months[] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

            char dateStr[32];
            char timeStr[16];
            snprintf(dateStr, sizeof(dateStr), "%s, %d %s %d", weekDays[data_dayOfWeek % 7], data_dayOfMonth, months[data_month % 13], data_year);
            snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", data_hours, data_minutes, data_seconds);

            Graphics_drawStringCentered(&g_sContext, (int8_t *)dateStr, AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
            Graphics_drawStringCentered(&g_sContext, (int8_t *)timeStr, AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);
            break;
        }
        case STATE_STOPWATCH: {
            // Handle the timer functions, display it on LCD
            if (button_s2_pressed) {
                if (stopwatchActive) {
                    stopwatch_stop();
                } else {
                    stopwatch_start();
                }
            }
            uint16_t seconds = stopwatchCount / 10;
            uint16_t tenths = stopwatchCount % 10;
            char buffer[32];
            sprintf(buffer, "%d.%d s", seconds, tenths);
            Graphics_drawStringCentered(&g_sContext, (int8_t *)"STOPWATCH", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
            Graphics_drawStringCentered(&g_sContext, (int8_t *)buffer, AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);

            break;
        }
        case STATE_TIMER: {
            handleTimerStateChange();
            handleTimerState();

            break;
        }
        case STATE_STEP_COUNT: {
            // Handle step counting, display it on LCD
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%d steps", stepCount);
            Graphics_drawStringCentered(&g_sContext, (int8_t *)"STEP COUNTER", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
            Graphics_drawStringCentered(&g_sContext, (int8_t *)buffer, AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);
            break;
        }
        case STATE_TEMPERATURE: {
            // Handle the temperature reader, display on LCD
            double t = TMP006_getTemp();
            if (button_s2_pressed) {
                clear_lcd(g_sContext);
                if (currentTemperatureFormat == CELSIUS)
                    currentTemperatureFormat = FARENHEIT;
                else
                    currentTemperatureFormat = CELSIUS;
            }
            char buffer[32];
            if (currentTemperatureFormat == CELSIUS) {
                double new_t = convert_temperature(t);
                snprintf(buffer, sizeof(buffer), "%.1f C", new_t);
            } else {
                snprintf(buffer, sizeof(buffer), "%.1f F", t);
            }
            Graphics_drawStringCentered(&g_sContext, (int8_t *)"TEMPERATURE", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
            Graphics_drawStringCentered(&g_sContext, (int8_t *)buffer, AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);

            break;
        }
    }
}

void handleTimerState(void) {
    switch (currentTimerState) {
        case INIT: {
            stopwatchCount = 0;
            if (joystickDataReady) {
                joystickDataReady = 0;
                joystickDirection = decodeJoystickDirection(joystickCoordBuffer[0], joystickCoordBuffer[1]);
            }
            if (currentTimerSet == SEC) {
                if (joystickDirection == 1) {
                    if (timerSeconds < 59)
                        timerSeconds++;
                    else
                        timerSeconds = 0;
                } else if (joystickDirection == 3) {
                    if (timerSeconds == 0)
                        timerSeconds = 59;
                    else
                        timerSeconds--;
                }
                if (button_s1_pressed) currentTimerSet = MIN;
            } else {
                if (joystickDirection == 1) {
                    if (timerMinutes < 59)
                        timerMinutes++;
                    else
                        timerMinutes = 0;
                } else if (joystickDirection == 3) {
                    if (timerMinutes == 0)
                        timerMinutes = 59;
                    else
                        timerMinutes--;
                }
                if (button_s1_pressed) currentTimerSet = SEC;
            }
            totalTicks = (timerSeconds + 60 * timerMinutes) * 10;
            char buffer[32];
            sprintf(buffer, "%02d:%02d:00", timerMinutes, timerSeconds);
            Graphics_drawStringCentered(&g_sContext, (int8_t *)"TIMER", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
            Graphics_drawStringCentered(&g_sContext, (int8_t *)buffer, AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);

            break;
        }
        case ACTIVE: {
            if (!stopwatchActive) stopwatch_start();
            ticksLeft = totalTicks - stopwatchCount;

            uint8_t tmp = ticksLeft / 10;
            uint8_t tenths = (ticksLeft % 10) * 10;
            uint8_t seconds = tmp % 60;
            uint8_t minutes = tmp / 60;

            char buffer[32];
            sprintf(buffer, "%02d:%02d:%02d", minutes, seconds, tenths);
            Graphics_drawStringCentered(&g_sContext, (int8_t *)"TIMER", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
            Graphics_drawStringCentered(&g_sContext, (int8_t *)buffer, AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);

            break;
        }
        case STOP: {
            if (stopwatchActive) stopwatch_stop();
            break;
        }
        case END: {
            if (stopwatchActive) stopwatch_stop();
            Graphics_drawStringCentered(&g_sContext, (int8_t *)"TIMER", AUTO_STRING_LENGTH, 64, 50, OPAQUE_TEXT);
            Graphics_drawStringCentered(&g_sContext, (int8_t *)"TIME IS UP!", AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);
            break;
        }
    }
}

int main(void) {
    _hwInit();

    while (1) {
        PCM_gotoLPM0();
        if (joystickDataReady) {
            joystickDataReady = 0;
            joystickDirection = decodeJoystickDirection(joystickCoordBuffer[0], joystickCoordBuffer[1]);

            handleStateChange(joystickDirection);
        }
        handleSystemState();
    }
}
