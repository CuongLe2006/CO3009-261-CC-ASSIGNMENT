/*
 * lcd.h
 *
 *  Created on: 21 thg 11, 2026
 *      Author: Windows
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_
#include "main.h"

void lcd_init(void);
void lcd_print_row(uint8_t row  , const char* str);
void lcd_task_env(void);

#endif /* INC_LCD_H_ */
