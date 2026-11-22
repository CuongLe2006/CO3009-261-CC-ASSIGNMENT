#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

typedef enum { LIGHT_RED, LIGHT_GREEN, LIGHT_AMBER, LIGHT_OFF } Light_t;

void traffic_set(int road, Light_t color);   /* road = 1 hoac 2 */
Light_t traffic_get(int road);               /* màu đã set gần nhất */
#endif
