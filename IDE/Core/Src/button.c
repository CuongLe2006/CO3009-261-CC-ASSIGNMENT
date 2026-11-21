#include "button.h"
#include "main.h"

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
        r2[i] = r1[i];
        r1[i] = r0[i];
        r0[i] = (HAL_GPIO_ReadPin(btn_port[i], btn_pin[i]) == BTN_PRESSED_LEVEL);
        if (r0[i] == r1[i] && r1[i] == r2[i]) {
            if (r0[i] && !stable[i]) pressed_flag[i] = 1;   /* canh nhan */
            stable[i] = r0[i];
        }
    }
}

uint8_t isButtonPressed(int idx) {
    if (idx < 0 || idx >= NO_OF_BUTTONS) return 0;
    if (pressed_flag[idx]) { pressed_flag[idx] = 0; return 1; }
    return 0;
}
