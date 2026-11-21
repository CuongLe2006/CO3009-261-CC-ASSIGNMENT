#ifndef INC_FSM_MODE_H_
#define INC_FSM_MODE_H_

#include "global.h"

void fsm_mode_init(void);
void fsm_mode_run(void);                   /* task 10 ms */
void fsm_mode_on_change(Mode_t new_mode);  /* HOOK weak: ban LCD ghi de de hien ten mode */

#endif
