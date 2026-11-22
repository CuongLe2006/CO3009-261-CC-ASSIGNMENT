#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include <stdint.h>

#define NO_OF_BUTTONS 4        /* index 0..3 = BTN1..BTN4 */

void    button_scan(void);              /* task 1 tick (10 ms) */
uint8_t isButtonPressed(int idx);       /* tra 1 DUNG MOT LAN moi lan nhan */
uint8_t isButtonLongPressed(int idx);
uint8_t isButtonRepeat(int idx);

#endif
