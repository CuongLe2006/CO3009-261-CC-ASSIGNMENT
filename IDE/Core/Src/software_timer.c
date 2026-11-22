#include "software_timer.h"
#include "scheduler.h"

volatile int timer_flag[MAX_TIMER];
static volatile int timer_counter[MAX_TIMER];

void setTimer(int idx, int duration_ms) {
    if (idx < 0 || idx >= MAX_TIMER) return;
    timer_counter[idx] = duration_ms / TICK_MS;
    timer_flag[idx]    = (timer_counter[idx] <= 0) ? 1 : 0;
}

void timer_clear_all(void) {
    for (int i = 0; i < MAX_TIMER; i++) {
        timer_counter[i] = 0;
        timer_flag[i]    = 0;
    }
}

int getTimerLeft(int idx) {
    if (idx < 0 || idx >= MAX_TIMER) return 0;
    return timer_counter[idx] * TICK_MS;
}

void timer_run(void) {
    for (int i = 0; i < MAX_TIMER; i++) {
        if (timer_counter[i] > 0) {
            timer_counter[i]--;
            if (timer_counter[i] == 0) timer_flag[i] = 1;
        }
    }
}

int timer_get_left_ms(int idx) {
    if (idx < 0 || idx >= MAX_TIMER) return 0;
    return timer_counter[idx] * TICK_MS;
}
