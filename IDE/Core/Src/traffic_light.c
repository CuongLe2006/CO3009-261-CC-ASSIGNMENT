#include "traffic_light.h"
#include "main.h"

/* {A, B} cho tung mau, lay tu bang to hop tin hieu cua nhom (muc 3.2 bao cao) */
static const uint8_t light_code[4][2] = {
    [LIGHT_RED]   = {1, 1},
    [LIGHT_GREEN] = {1, 0},
    [LIGHT_AMBER] = {0, 1},
    [LIGHT_OFF]   = {0, 0},
};

void traffic_set(int road, Light_t color) {
    GPIO_PinState a = light_code[color][0] ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState b = light_code[color][1] ? GPIO_PIN_SET : GPIO_PIN_RESET;
    if (road == 1) {
        HAL_GPIO_WritePin(TL1A_GPIO_Port, TL1A_Pin, a);
        HAL_GPIO_WritePin(TL1B_GPIO_Port, TL1B_Pin, b);
    } else if (road == 2) {
        HAL_GPIO_WritePin(TL2A_GPIO_Port, TL2A_Pin, a);
        HAL_GPIO_WritePin(TL2B_GPIO_Port, TL2B_Pin, b);
    }
}
