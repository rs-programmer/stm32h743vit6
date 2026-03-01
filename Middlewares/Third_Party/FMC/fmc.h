#ifndef __FMC_H__
#define __FMC_H__

#include "fmc_conf.h"

#include "cmsis_os.h"
#include "lcd.h"
#include "main.h"
#include "stm32h7xx_hal_def.h"

typedef struct {
    uint32_t cmd;
    union {
        lcd_msg_t lcd_msg;
    } u;
} fmc_msg_t;

extern osMessageQueueId_t fmc_queue;

HAL_StatusTypeDef MX_FMC_Init(void);

HAL_StatusTypeDef MX_FMC_SendMsg(fmc_msg_t *msg);

#endif /* __FMC_H__ */
