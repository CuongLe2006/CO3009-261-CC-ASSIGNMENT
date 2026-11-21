#include "fsm_tuning.h"
#include "traffic_light.h"

/* Tam thoi - ban lam tuning se thay */
void fsm_tuning_init(void) { traffic_set(1, LIGHT_OFF); traffic_set(2, LIGHT_OFF); }
void fsm_tuning_run(void)  { }
