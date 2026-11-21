#include "fsm_mode.h"
#include "global.h"
#include "button.h"
#include "software_timer.h"
#include "traffic_light.h"
#include "fsm_automatic.h"
#include "fsm_manual.h"
#include "fsm_tuning.h"

/* Mac dinh khong lam gi. Ban LCD dinh nghia ham cung ten (khong weak) de ghi de. */
__attribute__((weak)) void fsm_mode_on_change(Mode_t new_mode) {
    (void)new_mode;
}

/*
 * Moi lan doi mode deu di qua day:
 *  1. Ep CA HAI duong ve DO  -> khong bao gio 2 duong cung xanh
 *  2. Huy moi software timer -> khong con timer cu cua mode truoc
 *  3. Goi *_init cua mode moi
 *  4. Bao LCD (hook)
 */
static void enter_mode(Mode_t m) {
    traffic_set(1, LIGHT_RED);
    traffic_set(2, LIGHT_RED);
    timer_clear_all();
    mode = m;
    switch (m) {
    case MODE_AUTO:   fsm_automatic_init(); break;
    case MODE_MANUAL: fsm_manual_init();    break;
    case MODE_TUNING: fsm_tuning_init();    break;
    }
    fsm_mode_on_change(m);
}

void fsm_mode_init(void) {
    enter_mode(MODE_AUTO);
}

void fsm_mode_run(void) {
    if (isButtonPressed(0)) {                    /* BTN1 = nut doi mode */
        switch (mode) {
        case MODE_AUTO:   enter_mode(MODE_MANUAL); break;
        case MODE_MANUAL: enter_mode(MODE_TUNING); break;
        case MODE_TUNING: enter_mode(MODE_AUTO);   break;
        }
    }

    switch (mode) {
    case MODE_AUTO:   fsm_automatic_run(); break;
    case MODE_MANUAL: fsm_manual_run();    break;
    case MODE_TUNING: fsm_tuning_run();    break;
    }
}
