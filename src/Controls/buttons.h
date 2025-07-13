#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stdint.h>
#include <stdbool.h>

extern volatile uint8_t button_s1_pressed;
extern volatile uint8_t button_s2_pressed;

void buttons_init(void);

#endif /* BUTTONS_H_ */
