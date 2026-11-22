#include "fsm_tuning.h"
#include "global.h"
#include "button.h"
#include "software_timer.h"
#include "traffic_light.h"

#define TUNE_ABS_MAX  99
#define BLINK_MS      250                    /* 2 Hz */

static TuneField_t field;
static int val;                              /* bản nháp (giây) */
static uint8_t blink_on;

static int s_red(void)   { return (int)(time_red   / 1000); }
static int s_green(void) { return (int)(time_green / 1000); }
static int s_amber(void) { return (int)(time_amber / 1000); }

static int get_min(void) {
    return (field == TUNE_RED) ? s_amber() + 1 : 1;
}

static int get_max(void) {
    switch (field) {
    case TUNE_RED:   return TUNE_ABS_MAX;
    case TUNE_AMBER: return TUNE_ABS_MAX - s_green();
    default:         return TUNE_ABS_MAX - s_amber();      /* GREEN */
    }
}

static void load_val(void) {
    switch (field) {
    case TUNE_RED:   val = s_red();   break;
    case TUNE_AMBER: val = s_amber(); break;
    default:         val = s_green(); break;
    }
}

static Light_t field_color(void) {
    switch (field) {
    case TUNE_RED:   return LIGHT_RED;
    case TUNE_AMBER: return LIGHT_AMBER;
    default:         return LIGHT_GREEN;
    }
}

static void show_lights(void) {
    Light_t c = blink_on ? field_color() : LIGHT_OFF;
    traffic_set(1, c);                       /* cả hai đường cùng nháy */
    traffic_set(2, c);
}

static void restart_blink(void) {
    blink_on = 1;
    setTimer(TMR_TUNING, BLINK_MS);
    show_lights();
}

/* Ghi bản nháp vào time_*, luôn giữ red = green + amber */
static void commit(void) {
    switch (field) {
    case TUNE_RED:
        time_red   = (uint32_t)val * 1000;
        time_green = time_red - time_amber;
        break;
    case TUNE_AMBER:
        time_amber = (uint32_t)val * 1000;
        time_red   = time_amber + time_green;
        break;
    default:                                 /* TUNE_GREEN */
        time_green = (uint32_t)val * 1000;
        time_red   = time_amber + time_green;
        break;
    }
}

void fsm_tuning_init(void) {                 /* fsm_mode đã đưa 2 đường về ĐỎ + xoá timer */
    field = TUNE_RED;
    load_val();                              /* bản nháp luôn khởi tạo lại -> BTN1 thoát = bỏ giá trị chưa lưu */
    restart_blink();
}

void fsm_tuning_run(void) {
    if (isButtonPressed(2) || isButtonRepeat(2)) {   /* BTN3: +1 s, giữ nút thì tự tăng */
        if (val < get_max()) val++;
    }
    if (isButtonPressed(3) || isButtonRepeat(3)) {   /* BTN4: -1 s, giữ nút thì tự giảm */
        if (val > get_min()) val--;
    }
    if (isButtonPressed(1)) {                /* BTN2: lưu rồi sang trường kế */
        commit();
        field = (field == TUNE_GREEN) ? TUNE_RED : (TuneField_t)(field + 1);
        load_val();
        restart_blink();
    }
    if (timer_flag[TMR_TUNING]) {            /* nháy đèn */
        blink_on ^= 1;
        setTimer(TMR_TUNING, BLINK_MS);
        show_lights();
    }
}

TuneField_t fsm_tuning_get_field(void) { return field; }
int         fsm_tuning_get_val(void)   { return val; }
