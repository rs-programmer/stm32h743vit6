#include "rng.h"

RNG_HandleTypeDef hrng1 = {0};
uint32_t hrng1_random = 0;
bool hrng1_complete = false;

HAL_StatusTypeDef MX_RNG_Init(void) {
    HAL_StatusTypeDef ret = HAL_OK;

    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
    PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_PLL;
    ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    if (ret != HAL_OK) {
        Error_Handler();
    }
    __HAL_RCC_RNG_CLK_ENABLE();

    hrng1.Instance = RNG;
    hrng1.Init.ClockErrorDetection = RNG_CED_ENABLE;
    ret = HAL_RNG_Init(&hrng1);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(RNG_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(RNG_IRQn);
    return ret;
}

HAL_StatusTypeDef MX_RNG_RandFull(uint8_t *rand_buffer, uint32_t len) {
    HAL_StatusTypeDef ret = HAL_OK;
    uint32_t random32bit = 0;
    for (int i = 0; i < len; i++) {
        ret = rng_rand(random32bit);
        if (ret != HAL_OK) {
            Error_Handler();
        }
        rand_buffer[i] = (uint8_t)(random32bit & 0xFF);
    }

    return ret;
}

void RNG_IRQHandler(void) { HAL_RNG_IRQHandler(&hrng1); }

void HAL_RNG_ReadyDataCallback(RNG_HandleTypeDef *hrng, uint32_t random32bit) {
    if (hrng == &hrng1) {
        hrng1_complete = true;
        hrng1_random = random32bit;
    }
}