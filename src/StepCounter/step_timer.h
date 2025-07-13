#ifndef step_timer_H_
#define step_timer_H_

#include <stdint.h>
#include <stdbool.h>
bool readyNextStep;

void step_timer_init(void);
void TA2_0_IRQHandler(void);

#endif /* step_timer_H_ */
