#ifndef stopwatch_H_
#define stopwatch_H_

#include <stdint.h>
#include <stdbool.h>
extern bool stopwatchActive;

extern uint16_t stopwatchCount;

extern uint16_t timerMinutes;
extern uint16_t timerSeconds;
extern uint16_t totalTicks;
extern uint16_t ticksLeft;

void stopwatch_init(void);
void TA0_0_IRQHandler(void);
void stopwatch_start(void);
void stopwatch_stop(void);


#endif /* stopwatch_H_ */
