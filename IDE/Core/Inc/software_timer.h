#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define MAX_TIMER 5

extern volatile int timer_flag[MAX_TIMER];

void setTimer(int idx, int duration_ms);
void timer_clear_all(void);
void timer_run(void);            /* task chu ky 1 tick (10 ms) */

#endif
