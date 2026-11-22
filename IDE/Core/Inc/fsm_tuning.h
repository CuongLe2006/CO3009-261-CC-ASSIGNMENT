#ifndef INC_FSM_TUNING_H_
#define INC_FSM_TUNING_H_

typedef enum { TUNE_RED, TUNE_AMBER, TUNE_GREEN } TuneField_t;

void        fsm_tuning_init(void);
void        fsm_tuning_run(void);            /* gọi bởi fsm_mode_run khi mode == MODE_TUNING */
TuneField_t fsm_tuning_get_field(void);
int         fsm_tuning_get_val(void);        /* bản nháp, giây */

#endif
