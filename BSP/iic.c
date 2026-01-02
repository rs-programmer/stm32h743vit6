#include "iic.h"

HAL_StatusTypeDef MX_I2C1_Init(void) {
    HAL_StatusTypeDef ret = HAL_OK;

    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    PeriphClkInitStruct.RngClockSelection = RCC_I2C1CLKSOURCE_CSI;
    ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    if (ret != HAL_OK) {
        Error_Handler();
    }
    __HAL_RCC_I2C1_CLK_ENABLE();

    return ret;
}