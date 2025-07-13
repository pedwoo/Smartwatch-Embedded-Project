#include <Controls/joystick.h>
#include <StepCounter/step_counter.h>
#include <StepCounter/step_timer.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/inc/msp.h>


void adc_init() {
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

    // Configure GPIO pins for both joystick and accelerometer
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0 | GPIO_PIN1, GPIO_TERTIARY_MODULE_FUNCTION); // A15, A14
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION); // A11, A13, A9

    // Configure all conversion memories in a sequence
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM4, true);

    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS); // Joystick X
    ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);  // Joystick Y
    ADC14_configureConversionMemory(ADC_MEM2, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A14, ADC_NONDIFFERENTIAL_INPUTS); // Accel X
    ADC14_configureConversionMemory(ADC_MEM3, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A13, ADC_NONDIFFERENTIAL_INPUTS); // Accel Y
    ADC14_configureConversionMemory(ADC_MEM4, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A11, ADC_NONDIFFERENTIAL_INPUTS); // Accel Z

    ADC14_enableInterrupt(ADC_INT1 | ADC_INT4); // End of joystick and accel sequences
    Interrupt_enableInterrupt(INT_ADC14);
    Interrupt_enableMaster();

    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
}

void ADC14_IRQHandler(void)
{
    uint64_t status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    // ADC conversion for joystick input
    if (status & ADC_INT1)
    {
        joystickCoordBuffer[0] = ADC14_getResult(ADC_MEM0);
        joystickCoordBuffer[1] = ADC14_getResult(ADC_MEM1);
        joystickDataReady = 1;  // new raw data ready
    }

    // ADC conversion for accelerometer data
    if (status & ADC_INT4)
    {
        /* Store ADC14 conversion results */
        accelerometerBuffer[0] = ADC14_getResult(ADC_MEM2) - 8200;
        accelerometerBuffer[1] = ADC14_getResult(ADC_MEM3) - 8200;
        accelerometerBuffer[2] = ADC14_getResult(ADC_MEM4) - 8200;

        uint32_t mag = (accelerometerBuffer[0] * accelerometerBuffer[0]) +
                       (accelerometerBuffer[1] * accelerometerBuffer[1]) +
                       (accelerometerBuffer[2] * accelerometerBuffer[2]);
        mag = movingAverage(mag);
        if((mag > 15000000 || mag < 9000000) && readyNextStep){
            stepCount++;
            readyNextStep = false;
            Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);
        }
    }
}
