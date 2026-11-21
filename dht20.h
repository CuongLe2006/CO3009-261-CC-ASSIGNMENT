/*
 * dht20.h
 *
 *  Created on: 21 thg 11, 2026
 *      Author: Windows
 */

#ifndef INC_DHT20_H_
#define INC_DHT20_H_

#include "main.h"


void    dht20_init(void);          // gọi 1 lần trước scheduler
void    dht20_trigger(void);       // task: gửi lệnh đo
void    dht20_read(void);          // task one-shot: đọc kết quả
int32_t dht20_get_temp_x10(void);  // 253 = 25.3 °C
int32_t dht20_get_humi_x10(void);  // 652 = 65.2 %


#endif /* INC_DHT20_H_ */
