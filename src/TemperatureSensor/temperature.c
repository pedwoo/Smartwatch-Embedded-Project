#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <TemperatureSensor/HAL_I2C.h>
#include <TemperatureSensor/HAL_TMP006.h>


void temperature_init() {
//    /* Halting WDT and disabling master interrupts */
//    WDT_A_holdTimer();
//    Interrupt_disableMaster();

    /* Set the core voltage level to VCORE1 */
//    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */

    /* Temperature Sensor initialization */
    /* Initialize I2C communication */
    Init_I2C_GPIO();
    I2C_init();
    /* Initialize TMP006 temperature sensor */
    TMP006_init();
//    __delay_cycles(100000);
}

double convert_temperature(double f) {
    return (f - 32.0) * 5 / 9;
}
