#include "gpio.h"

void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    LED0_OFF();
    LED1_OFF();
}

#if 0
void MX_KEY_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* WAKE_UP PA0 */
    __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_0);
    HAL_NVIC_SetPriority(EXTI0_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    /* KEY1 PA1 */
    __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_1);
    HAL_NVIC_SetPriority(EXTI1_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_0) {
        printf("GPIO_PIN_0\r\n");
        LED0_TOGGLE();
    } else if (GPIO_Pin == GPIO_PIN_1) {
        printf("GPIO_PIN_1\r\n");
        LED1_TOGGLE();
    }
}
#endif