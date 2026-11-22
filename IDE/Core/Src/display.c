#include "display.h"
#include "lcd.h"
#include "global.h"
#include "fsm_mode.h"
#include "fsm_tuning.h"
#include "traffic_light.h"
#include "software_timer.h"
#include <stdio.h>

static const char *color_name(Light_t c) {
    switch (c) {
    case LIGHT_RED:   return "RED";
    case LIGHT_GREEN: return "GRN";
    case LIGHT_AMBER: return "AMB";
    default:          return "OFF";
    }
}

/* Giây còn lại của 1 đường, làm tròn lên.
   Đường đỏ: left + amber khi đường kia xanh, left khi đường kia vàng. */
static int road_secs(int road) {
    Light_t me    = traffic_get(road);
    Light_t other = traffic_get(road == 1 ? 2 : 1);
    uint32_t ms   = (uint32_t)timer_get_left_ms(TMR_AUTO);
    if (me == LIGHT_RED && other == LIGHT_GREEN) ms += time_amber;
    int s = (int)((ms + 999) / 1000);
    return s > 99 ? 99 : s;
}

void lcd_task_mode(void) {
    char line[17];
    switch (mode) {
    case MODE_AUTO:
        snprintf(line, sizeof line, "1:%s%02d 2:%s%02d",
                 color_name(traffic_get(1)), road_secs(1),
                 color_name(traffic_get(2)), road_secs(2));
        lcd_print_row(0, line);
        break;

    case MODE_MANUAL:
        snprintf(line, sizeof line, "1:%-3s   2:%-3s",
                 color_name(traffic_get(1)), color_name(traffic_get(2)));
        lcd_print_row(0, line);
        break;

    case MODE_TUNING: {
        static const char *fname[] = { "RED", "AMBER", "GREEN" };
        snprintf(line, sizeof line, "EDIT %-5s  %02ds",
                 fname[fsm_tuning_get_field()], fsm_tuning_get_val());
        lcd_print_row(0, line);
        snprintf(line, sizeof line, "R:%02d A:%02d G:%02d",
                 (int)(time_red / 1000), (int)(time_amber / 1000), (int)(time_green / 1000));
        lcd_print_row(1, line);
        break;
    }
    }
}

/* Ghi đè bản weak trong fsm_mode.c: vẽ lại LCD ngay khi đổi mode */
void fsm_mode_on_change(Mode_t new_mode) {
    (void)new_mode;
    lcd_task_mode();
    lcd_task_env();                          /* tự return nếu đang ở Tuning; ra khỏi Tuning thì trả hàng dưới về T/RH */
}
