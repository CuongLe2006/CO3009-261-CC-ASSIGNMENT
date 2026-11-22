#include "button.h"
#include "main.h"

#define LONG_PRESS_TICKS     100   /* 1 s   */
#define REPEAT_DELAY_TICKS    50   /* 0,5 s truoc khi bat dau lap */
#define REPEAT_PERIOD_TICKS   10   /* lap moi 0,1 s */

static uint16_t hold_cnt[NO_OF_BUTTONS];
static volatile uint8_t long_flag[NO_OF_BUTTONS];
static volatile uint8_t repeat_flag[NO_OF_BUTTONS];

/* Muc logic khi NHAN: GPIO_PIN_RESET (nhan = 0) hoac GPIO_PIN_SET (nhan = 1) */
#define BTN_PRESSED_LEVEL GPIO_PIN_RESET

static GPIO_TypeDef *btn_port[NO_OF_BUTTONS] = {BTN1_GPIO_Port, BTN2_GPIO_Port,
                                                BTN3_GPIO_Port, BTN4_GPIO_Port};
static const uint16_t btn_pin[NO_OF_BUTTONS] = {BTN1_Pin, BTN2_Pin, BTN3_Pin, BTN4_Pin};

static uint8_t r0[NO_OF_BUTTONS], r1[NO_OF_BUTTONS], r2[NO_OF_BUTTONS];
static uint8_t stable[NO_OF_BUTTONS];
static volatile uint8_t pressed_flag[NO_OF_BUTTONS];

/* Task 10 ms: debounce bang 3 lan doc lien tiep giong nhau */
void button_scan(void) {
    for (int i = 0; i < NO_OF_BUTTONS; i++) {
        if (stable[i]) {
            if (hold_cnt[i] < 60000) hold_cnt[i]++;            /* khong tran so */
            if (hold_cnt[i] == LONG_PRESS_TICKS) long_flag[i] = 1;
            if (hold_cnt[i] >= REPEAT_DELAY_TICKS &&
                (hold_cnt[i] - REPEAT_DELAY_TICKS) % REPEAT_PERIOD_TICKS == 0)
                repeat_flag[i] = 1;
        } else {
            hold_cnt[i] = 0;
        }
    }
}

uint8_t isButtonPressed(int idx) {
    if (idx < 0 || idx >= NO_OF_BUTTONS) return 0;
    if (pressed_flag[idx]) { pressed_flag[idx] = 0; return 1; }
    return 0;
}

uint8_t isButtonLongPressed(int idx) {
    if (idx < 0 || idx >= NO_OF_BUTTONS) return 0;
    if (long_flag[idx]) { long_flag[idx] = 0; return 1; }
    return 0;
}

uint8_t isButtonRepeat(int idx) {          /* 1 moi nhip lap khi dang giu nut */
    if (idx < 0 || idx >= NO_OF_BUTTONS) return 0;
    if (repeat_flag[idx]) { repeat_flag[idx] = 0; return 1; }
    return 0;
}
