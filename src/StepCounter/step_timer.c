#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

bool readyNextStep;

void step_timer_init(void){
    Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_32,
        56250,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
        TIMER_A_DO_CLEAR
    };
    Timer_A_configureUpMode(TIMER_A2_BASE, &upConfig);
    Interrupt_enableInterrupt(INT_TA2_0);
    Interrupt_enableMaster();
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);
    readyNextStep = false;
}

void TA2_0_IRQHandler(void){
    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    readyNextStep = true;
    Timer_A_stopTimer(TIMER_A2_BASE);
}
