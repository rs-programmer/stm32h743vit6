#include "exti.h"
#include "gpio.h"

EXTI_HandleTypeDef hexti0;
EXTI_HandleTypeDef hexti1;
EXTI_HandleTypeDef hexti13;

void HAL_EXTI0_Callback(void) {
    printf("HAL_EXTI0_Callback\r\n");
    LED0_TOGGLE();
}

void HAL_EXTI1_Callback(void) {
    printf("HAL_EXTI1_Callback\r\n");
    LED1_TOGGLE();
}

void HAL_EXTI13_Callback(void) {
    printf("HAL_EXTI13_Callback\r\n");
    LED1_TOGGLE();
}

HAL_StatusTypeDef MX_EXTI0_Init(void) {
    HAL_StatusTypeDef ret = HAL_OK;

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    EXTI_ConfigTypeDef exti0_config = {
        .Line = EXTI_LINE_0,
        .Mode = EXTI_MODE_INTERRUPT,
        .Trigger = EXTI_TRIGGER_FALLING,
        .GPIOSel = EXTI_GPIOA,
        .PendClearSource = EXTI_D3_PENDCLR_SRC_NONE,
    };
    ret = HAL_EXTI_SetConfigLine(&hexti0, &exti0_config);
    if (ret != HAL_OK) {
        return ret;
    }

    ret = HAL_EXTI_RegisterCallback(
        &hexti0, HAL_EXTI_COMMON_CB_ID, HAL_EXTI0_Callback);
    if (ret != HAL_OK) {
        return ret;
    }

    HAL_EXTI_ClearPending(&hexti0, EXTI_TRIGGER_FALLING);
    HAL_NVIC_SetPriority(EXTI0_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    return ret;
}

HAL_StatusTypeDef MX_EXTI1_Init(void) {
    HAL_StatusTypeDef ret = HAL_OK;

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    EXTI_ConfigTypeDef exti1_config = {
        .Line = EXTI_LINE_1,
        .Mode = EXTI_MODE_INTERRUPT,
        .Trigger = EXTI_TRIGGER_FALLING,
        .GPIOSel = EXTI_GPIOA,
        .PendClearSource = EXTI_D3_PENDCLR_SRC_NONE,
    };
    ret = HAL_EXTI_SetConfigLine(&hexti1, &exti1_config);
    if (ret != HAL_OK) {
        return ret;
    }

    ret = HAL_EXTI_RegisterCallback(
        &hexti1, HAL_EXTI_COMMON_CB_ID, HAL_EXTI1_Callback);
    if (ret != HAL_OK) {
        return ret;
    }

    HAL_EXTI_ClearPending(&hexti1, EXTI_TRIGGER_FALLING);
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    return ret;
}

HAL_StatusTypeDef MX_EXTI13_Init(void) {
    HAL_StatusTypeDef ret = HAL_OK;

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    EXTI_ConfigTypeDef exti13_config = {
        .Line = EXTI_LINE_13,
        .Mode = EXTI_MODE_INTERRUPT,
        .Trigger = EXTI_TRIGGER_FALLING,
        .GPIOSel = EXTI_GPIOC,
        .PendClearSource = EXTI_D3_PENDCLR_SRC_NONE,
    };
    ret = HAL_EXTI_SetConfigLine(&hexti13, &exti13_config);
    if (ret != HAL_OK) {
        return ret;
    }

    ret = HAL_EXTI_RegisterCallback(
        &hexti13, HAL_EXTI_COMMON_CB_ID, HAL_EXTI13_Callback);
    if (ret != HAL_OK) {
        return ret;
    }

    HAL_EXTI_ClearPending(&hexti13, EXTI_TRIGGER_FALLING);
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    return ret;
}

void EXTI0_IRQHandler(void) {
    /* WAKE_UP */
    HAL_EXTI_IRQHandler(&hexti0);
}

void EXTI1_IRQHandler(void) {
    /* KEY1 */
    HAL_EXTI_IRQHandler(&hexti1);
}

void EXTI15_10_IRQHandler(void) {
    /* KEY */
    if (HAL_EXTI_GetPending(&hexti13, EXTI_TRIGGER_FALLING)) {
        HAL_EXTI_IRQHandler(&hexti13);
    }
}
