/*
 * lcd.c
 *
 *  Created on: 21 thg 11, 2026
 *      Author: Windows
 */

#include "lcd.h"
#include "dht20.h"
#include "global.h"
#include <string.h>
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;

#define LCD_ADDR  (0x27 << 1)   // đổi theo kết quả quét I2C
#define LCD_BL    0x08          // backlight
#define LCD_EN    0x04
#define LCD_RS    0x01

/* Nội dung đang hiển thị trên từng hàng, để chỉ vẽ lại khi có thay đổi */
static char lcd_cache[2][17];

/* Gửi 1 byte (lệnh hoặc dữ liệu) = 2 nibble, mỗi nibble có xung EN */
static void lcd_send(uint8_t data, uint8_t rs)
{
    uint8_t hi = (data & 0xF0)        | LCD_BL | rs;
    uint8_t lo = ((data << 4) & 0xF0) | LCD_BL | rs;
    uint8_t buf[4] = { hi | LCD_EN, hi, lo | LCD_EN, lo };
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, buf, 4, 10);
}

static void lcd_cmd(uint8_t c)  { lcd_send(c, 0); }
static void lcd_data(uint8_t d) { lcd_send(d, LCD_RS); }

void lcd_init(void)
{
    HAL_Delay(50);                       // chờ LCD ổn định sau khi cấp nguồn
    /* Chuỗi khởi tạo 4 bit theo datasheet HD44780: gửi 0x30 ba lần */
    uint8_t b[2];
    for (int i = 0; i < 3; i++) {
        b[0] = 0x30 | LCD_BL | LCD_EN;
        b[1] = 0x30 | LCD_BL;
        HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, b, 2, 10);
        HAL_Delay(5);
    }
    /* Chuyển sang chế độ 4 bit */
    b[0] = 0x20 | LCD_BL | LCD_EN;
    b[1] = 0x20 | LCD_BL;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, b, 2, 10);
    HAL_Delay(5);

    lcd_cmd(0x28);   // 4 bit, 2 dòng, font 5x8
    lcd_cmd(0x0C);   // bật hiển thị, tắt con trỏ
    lcd_cmd(0x06);   // con trỏ tự tăng
    lcd_cmd(0x01);   // xoá màn hình
    HAL_Delay(2);    // lệnh xoá cần ~1.6 ms

    /* Sau khi xoá màn hình, cả hai hàng đều là khoảng trắng */
    for (int r = 0; r < 2; r++) {
        memset(lcd_cache[r], ' ', 16);
        lcd_cache[r][16] = '\0';
    }
}

void lcd_print_row(uint8_t row, const char *str)
{
    if (row > 1) return;

    /* Chuẩn bị đủ 16 ký tự, đệm khoảng trắng phía sau */
    char line[17];
    uint8_t i = 0;
    while (i < 16 && str[i]) { line[i] = str[i]; i++; }
    while (i < 16) line[i++] = ' ';
    line[16] = '\0';

    if (strcmp(line, lcd_cache[row]) == 0) return;   // nội dung không đổi -> bỏ qua
    strcpy(lcd_cache[row], line);

    lcd_cmd((row == 0) ? 0x80 : 0xC0);               // đặt con trỏ về đầu hàng
    for (i = 0; i < 16; i++) {
        lcd_data((uint8_t)line[i]);
    }
}

void lcd_task_env(void)   // task chu kỳ, ví dụ 1000 ms
{
    if (mode == MODE_TUNING) return;   // hàng dưới đang dành cho R/A/G

    char line[17];
    int32_t t = dht20_get_temp_x10();
    int32_t h = dht20_get_humi_x10();
    snprintf(line, sizeof line, "T:%2ld.%ldC RH:%2ld.%ld%%",
             (long)(t / 10), (long)(t % 10),
             (long)(h / 10), (long)(h % 10));
    lcd_print_row(1, line);   // hàng dưới
}
