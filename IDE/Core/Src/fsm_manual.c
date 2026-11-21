#include "fsm_manual.h"
#include "traffic_light.h"
#include "button.h"

#define BTN_NEXT 1   /* BTN2 = chuyển trạng thái (BTN1 là nút đổi mode của fsm_mode) */

typedef enum { M_ALL_RED, M_R1_G2, M_R1_A2, M_G1_R2, M_A1_R2 } manual_state_t;
static manual_state_t state;

static void apply(void) {
    switch (state) {
        case M_ALL_RED: traffic_set(1, LIGHT_RED);   traffic_set(2, LIGHT_RED);   break;
        case M_R1_G2:   traffic_set(1, LIGHT_RED);   traffic_set(2, LIGHT_GREEN); break;
        case M_R1_A2:   traffic_set(1, LIGHT_RED);   traffic_set(2, LIGHT_AMBER); break;
        case M_G1_R2:   traffic_set(1, LIGHT_GREEN); traffic_set(2, LIGHT_RED);   break;
        case M_A1_R2:   traffic_set(1, LIGHT_AMBER); traffic_set(2, LIGHT_RED);   break;
    }
}

void fsm_manual_init(void) { state = M_ALL_RED; apply(); }

void fsm_manual_run(void) {
    if (isButtonPressed(BTN_NEXT)) {
        switch (state) {
            case M_ALL_RED: state = M_R1_G2; break;
            case M_R1_G2:   state = M_R1_A2; break;
            case M_R1_A2:   state = M_G1_R2; break;
            case M_G1_R2:   state = M_A1_R2; break;
            case M_A1_R2:   state = M_R1_G2; break;   /* quay vòng, không về ALL_RED */
        }
        apply();
    }
}
