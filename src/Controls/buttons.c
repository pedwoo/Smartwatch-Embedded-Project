//#include <sensors/buttons.h>
//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
//
//#define BUTTON_S1_PORT GPIO_PORT_P5
//#define BUTTON_S1_PIN  GPIO_PIN1
//
//#define BUTTON_S2_PORT GPIO_PORT_P3
//#define BUTTON_S2_PIN  GPIO_PIN5
//
//volatile uint8_t button_s1_pressed = 0;
//volatile uint8_t button_s2_pressed = 0;
//
//void buttons_init(void) {
//    // Configure S1 (P5.1) as input with pull-up resistor
//    GPIO_setAsInputPinWithPullUpResistor(BUTTON_S1_PORT, BUTTON_S1_PIN);
//    GPIO_interruptEdgeSelect(BUTTON_S1_PORT, BUTTON_S1_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
//    GPIO_clearInterruptFlag(BUTTON_S1_PORT, BUTTON_S1_PIN);
//    GPIO_enableInterrupt(BUTTON_S1_PORT, BUTTON_S1_PIN);
//
//    // Configure S2 (P3.5) as input with pull-up resistor
//    GPIO_setAsInputPinWithPullUpResistor(BUTTON_S2_PORT, BUTTON_S2_PIN);
//    GPIO_interruptEdgeSelect(BUTTON_S2_PORT, BUTTON_S2_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
//    GPIO_clearInterruptFlag(BUTTON_S2_PORT, BUTTON_S2_PIN);
//    GPIO_enableInterrupt(BUTTON_S2_PORT, BUTTON_S2_PIN);
//
//    // Enable the interrupts in NVIC
//    Interrupt_enableInterrupt(INT_PORT5);
//    Interrupt_enableInterrupt(INT_PORT3);
//}
//
//// ISR for Port 5 (S1: P5.1)
//void PORT5_IRQHandler(void) {
//    uint32_t status = GPIO_getEnabledInterruptStatus(BUTTON_S1_PORT);
//    GPIO_clearInterruptFlag(BUTTON_S1_PORT, status);
//
//    if (status & BUTTON_S1_PIN) button_s1_pressed = 1;
//}
//
//// ISR for Port 3 (S2: P3.5)
//void PORT3_IRQHandler(void) {
//    uint32_t status = GPIO_getEnabledInterruptStatus(BUTTON_S2_PORT);
//    GPIO_clearInterruptFlag(BUTTON_S2_PORT, status);
//
//    if (status & BUTTON_S2_PIN) button_s2_pressed = 1;
//}

#include <Controls/buttons.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define BUTTON_S1_PORT GPIO_PORT_P5
#define BUTTON_S1_PIN  GPIO_PIN1

#define BUTTON_S2_PORT GPIO_PORT_P3
#define BUTTON_S2_PIN  GPIO_PIN5

volatile uint8_t button_s1_pressed = 0;
volatile uint8_t button_s2_pressed = 0;

void buttons_init(void) {
    // Configure S1 (P5.1) as input with pull-up resistor
    GPIO_setAsInputPinWithPullUpResistor(BUTTON_S1_PORT, BUTTON_S1_PIN);
    GPIO_interruptEdgeSelect(BUTTON_S1_PORT, BUTTON_S1_PIN, GPIO_HIGH_TO_LOW_TRANSITION); // start with press
    GPIO_clearInterruptFlag(BUTTON_S1_PORT, BUTTON_S1_PIN);
    GPIO_enableInterrupt(BUTTON_S1_PORT, BUTTON_S1_PIN);

    // Configure S2 (P3.5) as input with pull-up resistor
    GPIO_setAsInputPinWithPullUpResistor(BUTTON_S2_PORT, BUTTON_S2_PIN);
    GPIO_interruptEdgeSelect(BUTTON_S2_PORT, BUTTON_S2_PIN, GPIO_HIGH_TO_LOW_TRANSITION); // start with press
    GPIO_clearInterruptFlag(BUTTON_S2_PORT, BUTTON_S2_PIN);
    GPIO_enableInterrupt(BUTTON_S2_PORT, BUTTON_S2_PIN);

    // Enable the interrupts in NVIC
    Interrupt_enableInterrupt(INT_PORT5);
    Interrupt_enableInterrupt(INT_PORT3);
}

// ISR for Port 5 (S1: P5.1)
void PORT5_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(BUTTON_S1_PORT);
    GPIO_clearInterruptFlag(BUTTON_S1_PORT, status);

    if (status & BUTTON_S1_PIN) {
        if (GPIO_getInputPinValue(BUTTON_S1_PORT, BUTTON_S1_PIN) == 0) {
            // Pressed
            button_s1_pressed = 1;
            GPIO_interruptEdgeSelect(BUTTON_S1_PORT, BUTTON_S1_PIN, GPIO_LOW_TO_HIGH_TRANSITION); // next: release
        } else {
            // Released
            button_s1_pressed = 0;
            GPIO_interruptEdgeSelect(BUTTON_S1_PORT, BUTTON_S1_PIN, GPIO_HIGH_TO_LOW_TRANSITION); // next: press
        }
    }
}

// ISR for Port 3 (S2: P3.5)
void PORT3_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(BUTTON_S2_PORT);
    GPIO_clearInterruptFlag(BUTTON_S2_PORT, status);

    if (status & BUTTON_S2_PIN) {
        if (GPIO_getInputPinValue(BUTTON_S2_PORT, BUTTON_S2_PIN) == 0) {
            // Pressed
            button_s2_pressed = 1;
            GPIO_interruptEdgeSelect(BUTTON_S2_PORT, BUTTON_S2_PIN, GPIO_LOW_TO_HIGH_TRANSITION); // next: release
        } else {
            // Released
            button_s2_pressed = 0;
            GPIO_interruptEdgeSelect(BUTTON_S2_PORT, BUTTON_S2_PIN, GPIO_HIGH_TO_LOW_TRANSITION); // next: press
        }
    }
}
