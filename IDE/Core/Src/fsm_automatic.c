#include "fsm_automatic.h"
#include "global.h"
#include "software_timer.h"
#include "traffic_light.h"

/*
 * R1G2 (green) -> R1A2 (amber) -> G1R2 (green) -> A1R2 (amber) -> ...
 * Moi trang thai chi can 1 timer vi red = green + amber.
 * An toan: luon dua duong SAP MAT quyen ve DO truoc, roi moi bat XANH duong kia.
 */
void fsm_automatic_init(void) {
    auto_state = AUTO_INIT;
}

void fsm_automatic_run(void) {
    switch (auto_state) {

    case AUTO_INIT:
        traffic_set(1, LIGHT_RED);
        traffic_set(2, LIGHT_RED);
        traffic_set(2, LIGHT_GREEN);
        setTimer(TMR_AUTO, time_green);
        auto_state = AUTO_RED1_GREEN2;
        break;

    case AUTO_RED1_GREEN2:
        if (timer_flag[TMR_AUTO]) {
            traffic_set(2, LIGHT_AMBER);
            setTimer(TMR_AUTO, time_amber);
            auto_state = AUTO_RED1_AMBER2;
        }
        break;

    case AUTO_RED1_AMBER2:
        if (timer_flag[TMR_AUTO]) {
            traffic_set(2, LIGHT_RED);      /* do duong 2 truoc */
            traffic_set(1, LIGHT_GREEN);    /* roi moi xanh duong 1 */
            setTimer(TMR_AUTO, time_green);
            auto_state = AUTO_GREEN1_RED2;
        }
        break;

    case AUTO_GREEN1_RED2:
        if (timer_flag[TMR_AUTO]) {
            traffic_set(1, LIGHT_AMBER);
            setTimer(TMR_AUTO, time_amber);
            auto_state = AUTO_AMBER1_RED2;
        }
        break;

    case AUTO_AMBER1_RED2:
        if (timer_flag[TMR_AUTO]) {
            traffic_set(1, LIGHT_RED);      /* do duong 1 truoc */
            traffic_set(2, LIGHT_GREEN);    /* roi moi xanh duong 2 */
            setTimer(TMR_AUTO, time_green);
            auto_state = AUTO_RED1_GREEN2;
        }
        break;

    default:
        auto_state = AUTO_INIT;
        break;
    }
}
