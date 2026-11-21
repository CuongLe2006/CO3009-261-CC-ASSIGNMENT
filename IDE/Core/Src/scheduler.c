#include "scheduler.h"
#include "main.h"

static sTask    SCH_tasks_G[SCH_MAX_TASKS];
static uint8_t  SCH_count   = 0;
static uint32_t SCH_next_id = 1;

void SCH_Init(void) {
    SCH_count   = 0;
    SCH_next_id = 1;
}

/* Chen vao danh sach da sap theo thoi gian. Chi goi khi da tat ngat. */
static uint32_t sch_insert(void (*f)(void), uint32_t delay,
                           uint32_t period, uint32_t id) {
    if (SCH_count >= SCH_MAX_TASKS) return 0;

    uint32_t sum = 0;
    uint8_t  i   = 0;
    while (i < SCH_count && sum + SCH_tasks_G[i].Delay <= delay) {
        sum += SCH_tasks_G[i].Delay;
        i++;
    }
    uint32_t rel = delay - sum;                       /* delay tuong doi cua task moi */
    if (i < SCH_count) SCH_tasks_G[i].Delay -= rel;   /* task ngay sau bu lai */

    for (uint8_t j = SCH_count; j > i; j--) SCH_tasks_G[j] = SCH_tasks_G[j - 1];

    SCH_tasks_G[i].pTask  = f;
    SCH_tasks_G[i].Delay  = rel;
    SCH_tasks_G[i].Period = period;
    SCH_tasks_G[i].RunMe  = 0;
    SCH_tasks_G[i].TaskID = id;
    SCH_count++;
    return id;
}

uint32_t SCH_Add_Task(void (*pFunc)(void), uint32_t DELAY, uint32_t PERIOD) {
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    uint32_t id = sch_insert(pFunc, DELAY, PERIOD, SCH_next_id);
    if (id != 0) SCH_next_id++;
    __set_PRIMASK(primask);
    return id;                                        /* 0 = danh sach day */
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
    uint8_t ok = 0;
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    for (uint8_t i = 0; i < SCH_count; i++) {
        if (SCH_tasks_G[i].TaskID == taskID) {
            if (i + 1 < SCH_count) SCH_tasks_G[i + 1].Delay += SCH_tasks_G[i].Delay;
            for (uint8_t j = i; j + 1 < SCH_count; j++) SCH_tasks_G[j] = SCH_tasks_G[j + 1];
            SCH_count--;
            ok = 1;
            break;
        }
    }
    __set_PRIMASK(primask);
    return ok;
}

void SCH_Update(void) {                               /* O(1): chi dung task dau */
    if (SCH_count == 0) return;
    if (SCH_tasks_G[0].Delay > 0) SCH_tasks_G[0].Delay--;
    if (SCH_tasks_G[0].Delay == 0) SCH_tasks_G[0].RunMe = 1;
}

void SCH_Dispatch_Tasks(void) {
    if (SCH_count == 0 || SCH_tasks_G[0].RunMe == 0) return;

    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    sTask t = SCH_tasks_G[0];                         /* lay task dau ra */
    for (uint8_t j = 1; j < SCH_count; j++) SCH_tasks_G[j - 1] = SCH_tasks_G[j];
    SCH_count--;
    if (SCH_count > 0 && SCH_tasks_G[0].Delay == 0) SCH_tasks_G[0].RunMe = 1;
    __set_PRIMASK(primask);

    t.pTask();                                        /* chay task, ngoai vung tat ngat */

    if (t.Period > 0) {                               /* dinh ky: them lai, GIU NGUYEN ID */
        primask = __get_PRIMASK();
        __disable_irq();
        sch_insert(t.pTask, t.Period, t.Period, t.TaskID);
        __set_PRIMASK(primask);
    }                                                 /* one-shot: khong them lai */
}
