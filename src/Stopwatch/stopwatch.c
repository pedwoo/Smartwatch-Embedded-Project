#include <Stopwatch/stopwatch.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

bool stopwatchActive;
uint16_t stopwatchCount;

uint16_t timerMinutes;
uint16_t timerSeconds;
uint16_t totalTicks;
uint16_t ticksLeft;

void stopwatch_init(void){
    stopwatchCount = 0;
    timerMinutes = 1;
    timerSeconds = 0;
    totalTicks = 600;
    ticksLeft = 600;
    Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_8,
        37500,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
        TIMER_A_DO_CLEAR
    };
    Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
    Interrupt_enableInterrupt(INT_TA0_0);
    Interrupt_enableMaster();
    stopwatchActive = false;
}

void TA0_0_IRQHandler(void){
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    stopwatchCount++;
}

void stopwatch_start(void){
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
    stopwatchActive = true;
}

void stopwatch_stop(void){
    Timer_A_stopTimer(TIMER_A0_BASE);
    Timer_A_clearTimer(TIMER_A0_BASE);
    stopwatchActive = false;
}
