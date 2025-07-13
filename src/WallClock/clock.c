#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <WallClock/clock.h>

volatile uint8_t data_seconds;
volatile uint8_t data_minutes;
volatile uint8_t data_hours;
volatile uint8_t data_dayOfWeek;
volatile uint8_t data_dayOfMonth;
volatile uint8_t data_month;
volatile uint16_t data_year;

void date_init(void) {
    data_seconds = 23;
    data_minutes = 17;
    data_hours = 17;
    data_dayOfWeek = 1;
    data_dayOfMonth = 7;
    data_month = 7;
    data_year = 2025;


    Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,  // 3 MHz
        TIMER_A_CLOCKSOURCE_DIVIDER_64,  // 3 MHz / 64 = 46.875 kHz
        46874,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
        TIMER_A_DO_CLEAR
    };
    Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
    Interrupt_enableInterrupt(INT_TA1_0);
    Interrupt_enableMaster();
}

void TA1_0_IRQHandler(void) {
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    if(data_seconds == 59) {
        data_seconds = 0;
        if(data_minutes == 59) {
            data_minutes = 0;
            if(data_hours == 23) {
                data_hours = 0;
                data_dayOfWeek == (data_dayOfWeek + 1) % 7;
                data_dayOfMonth++;
            } else {
                data_hours++;
            }
        } else {
            data_minutes++;
        }
    } else {
        data_seconds++;
    }
}
