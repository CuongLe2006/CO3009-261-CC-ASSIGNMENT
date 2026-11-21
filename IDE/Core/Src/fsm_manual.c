#include "fsm_manual.h"
#include "traffic_light.h"

/* Tam thoi - ban lam manual se thay */
void fsm_manual_init(void) { traffic_set(1, LIGHT_RED); traffic_set(2, LIGHT_RED); }
void fsm_manual_run(void)  { }
