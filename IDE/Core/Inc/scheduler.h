#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>

#define SCH_MAX_TASKS 16
#define TICK_MS       10

typedef struct {
    void (*pTask)(void);
    uint32_t Delay;     /* tick con lai, TUONG DOI so voi task truoc */
    uint32_t Period;    /* 0 = one-shot */
    uint8_t  RunMe;
    uint32_t TaskID;
} sTask;

void     SCH_Init(void);
uint32_t SCH_Add_Task(void (*pFunc)(void), uint32_t DELAY, uint32_t PERIOD);
uint8_t  SCH_Delete_Task(uint32_t taskID);
void     SCH_Update(void);
void     SCH_Dispatch_Tasks(void);

#endif
