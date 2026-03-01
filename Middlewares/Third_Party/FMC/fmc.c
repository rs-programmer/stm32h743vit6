#include "fmc.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"

#define FMC_MSG_NUM (10)

osMessageQueueId_t fmc_queue = NULL;
osThreadId_t lcd_task_handle = NULL;

HAL_StatusTypeDef MX_FMC_Init(void) {
    fmc_queue = osMessageQueueNew(FMC_MSG_NUM, sizeof(fmc_msg_t), NULL);
    assert_param(fmc_queue != NULL);

    osThreadAttr_t attr = {
        .name = "fmc_task",
        .stack_size = 4096,
        .priority = osPriorityHigh,
    };

#if (FMC_LCD == 1)
    lcd_task_handle = osThreadNew(LCD_Task_Process, NULL, &attr);
    assert_param(lcd_task_handle != NULL);
#endif

    return HAL_OK;
}

HAL_StatusTypeDef MX_FMC_SendMsg(fmc_msg_t *msg) {
    osStatus_t ret = osMessageQueuePut(fmc_queue, msg, NULL, osWaitForever);
    return ret == osOK ? HAL_OK : HAL_ERROR;
}
