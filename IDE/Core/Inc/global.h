#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include <stdint.h>

typedef enum { MODE_AUTO, MODE_MANUAL, MODE_TUNING } Mode_t;

typedef enum {
    AUTO_INIT,
    AUTO_RED1_GREEN2,
    AUTO_RED1_AMBER2,
    AUTO_GREEN1_RED2,
    AUTO_AMBER1_RED2
} AutoState_t;

/* Moi FSM dung 1 software timer rieng */
#define TMR_AUTO    0
#define TMR_MANUAL  1
#define TMR_TUNING  2
#define TMR_LCD     3

extern Mode_t      mode;
extern AutoState_t auto_state;
extern uint32_t    time_red, time_green, time_amber;   /* ms, red = green + amber */

#endif
