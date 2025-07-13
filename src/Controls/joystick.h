#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>

// Globals to be used in main program
extern volatile uint16_t joystickCoordBuffer[2];
extern volatile uint8_t joystickDataReady;

extern uint8_t joystickDirection;

// Decode joystick direction from raw ADC x, y values
uint8_t decodeJoystickDirection(uint16_t x, uint16_t y);

// Interrupt handler declaration (optional, if you want to reference it explicitly)
void ADC14_IRQHandler(void);

#endif // JOYSTICK_H
