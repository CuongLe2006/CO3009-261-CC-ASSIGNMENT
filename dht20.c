/*
 * dht20.c
 *
 *  Created on: 21 thg 11, 2026
 *      Author: Windows
 */


#include "dht20.h"
#include "scheduler.h"

extern I2C_HandleTypeDef hi2c1;

#define DHT20_ADDR (0x38 << 1)

static int32_t temp_x10 = 0;
static int32_t humi_x10 = 0;

void dht20_init(void)
{
    HAL_Delay(100);   // DHT20 cần ≥100 ms sau khi cấp nguồn
    uint8_t status;
    HAL_I2C_Master_Receive(&hi2c1, DHT20_ADDR, &status, 1, 20);
    /* Nếu (status & 0x18) != 0x18 thì cần khởi tạo lại thanh ghi
       0x1B, 0x1C, 0x1E theo datasheet. Thực tế hiếm khi cần. */
}

/* Task chu kỳ (ví dụ 2000 ms = 200 tick) */
void dht20_trigger(void)
{
    uint8_t cmd[3] = { 0xAC, 0x33, 0x00 };
    HAL_I2C_Master_Transmit(&hi2c1, DHT20_ADDR, cmd, 3, 10);
    SCH_Add_Task(dht20_read, 10, 0);   // one-shot: chạy sau 100 ms (10 tick) – KHÔNG chặn CPU
}

void dht20_read(void)
{
    uint8_t d[7];
    if (HAL_I2C_Master_Receive(&hi2c1, DHT20_ADDR, d, 7, 20) != HAL_OK) return;
    if (d[0] & 0x80) return;   // còn bận, bỏ qua lần này (giữ giá trị cũ)

    uint32_t raw_h = ((uint32_t)d[1] << 12) | ((uint32_t)d[2] << 4) | (d[3] >> 4);
    uint32_t raw_t = ((uint32_t)(d[3] & 0x0F) << 16) | ((uint32_t)d[4] << 8) | d[5];

    humi_x10 = (int32_t)((raw_h * 1000UL) >> 20);
    temp_x10 = (int32_t)((raw_t * 2000UL) >> 20) - 500;
}

int32_t dht20_get_temp_x10(void) { return temp_x10; }
int32_t dht20_get_humi_x10(void) { return humi_x10; }
