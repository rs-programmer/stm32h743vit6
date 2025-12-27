#include "watchdog.h"

WWDG_HandleTypeDef hwwdg1 = {0};
IWDG_HandleTypeDef hiwdg1 = {0};

HAL_StatusTypeDef MX_WWDG_Init(void) {
    HAL_StatusTypeDef ret = HAL_OK;

    /* APB3 100MHZ */
    /** F = 100MHZ / 4096 / 128 = 190.73486328125 HZ WWDG实际输入时钟
     *  T = 1 / F = 5.24288 MS  每个计数值耗时
     *  W = T * (Counter - 64) = 5.24288 MS * (127 - 64) = 330.30144 MS 会触发中断
     *  W = T * (Counter - 64 + 1) = 5.24288 MS * (127 - 64 + 1) = 335.54432 MS 会触发复位
     */
    __HAL_RCC_WWDG1_CLK_ENABLE();

    hwwdg1.Instance = WWDG1;
    hwwdg1.Init.Prescaler = WWDG_PRESCALER_128;
    hwwdg1.Init.Window = WATCHDOG_WWDG_RELOAD;  /*   [64, 127] */
    hwwdg1.Init.Counter = WATCHDOG_WWDG_RELOAD; /**  [64, 127]，Counter 开始下降，
                                                     在 [Window, 64] 之间重新刷新就不会产生复位 */
    hwwdg1.Init.EWIMode = WWDG_EWI_ENABLE;      /*   Counter 达到 64 时触发中断 */
    ret = HAL_WWDG_Init(&hwwdg1);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    __HAL_WWDG_CLEAR_FLAG(&hwwdg1, WWDG_FLAG_EWIF);
    HAL_NVIC_SetPriority(WWDG_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
    return ret;
}

HAL_StatusTypeDef MX_IWDG_Init(void) {
    HAL_StatusTypeDef ret = HAL_OK;

    /* LSI 32KHZ 直连 IWDG 无需使能时钟寄存器 */
    /**
     * F = 32KHZ / IWDG_PRESCALER_256 = 125 HZ
     * T = 1 / F = 8 MS 每个计数值耗时
     * W = T * (Reload - 0) = 8 MS * (0xFFF - 0) = 32760 MS 会触发复位
     */

    hiwdg1.Instance = IWDG1;
    hiwdg1.Init.Prescaler = IWDG_PRESCALER_256;
    hiwdg1.Init.Reload = WATCHDOG_IWDG_RELOAD;
    hiwdg1.Init.Window = WATCHDOG_IWDG_RELOAD;
    ret = HAL_IWDG_Init(&hiwdg1);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    return ret;
}

void WWDG_IRQHandler(void) { HAL_WWDG_IRQHandler(&hwwdg1); }

void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg) {
    /* 提前唤醒中断回调函数: 可刷新Counter 重新启动 WWDG */
    wwdg_feed();
    /* 也可进行其他参数记录 */
    printf("HAL_WWDG_EarlyWakeupCallback\n");
}