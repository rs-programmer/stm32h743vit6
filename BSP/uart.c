#include "uart.h"

UART_HandleTypeDef huart1;

uint8_t uart1_rx_buffer[128];
bool uart1_rx_complete = false;

HAL_StatusTypeDef MX_UART1_Init(void) {
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART16;
    PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        Error_Handler();
    }

    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* DMA配置 */

    huart1.Instance = USART1;
    huart1.Init.BaudRate = 2000000;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; // 高级功能
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }

    /* FIFO 空闲个数大于等于 1/8 时触发中断 */
    if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) !=
        HAL_OK) {
        Error_Handler();
    }

    /* FIFO 接收数据个数大于等于 1/8 时触发中断 */
    if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) !=
        HAL_OK) {
        Error_Handler();
    }

    if (HAL_UARTEx_EnableFifoMode(&huart1) != HAL_OK) {
        Error_Handler();
    }

    /* 开启中断 */
    HAL_NVIC_SetPriority(USART1_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    return ret;
}

int fputc(int ch, FILE *f) {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}

int fgetc(FILE *f) {
    uint8_t ch = 0;
    HAL_UART_Receive(&huart1, &ch, 1, 0xFFFF);
    return ch;
}

/**
 * @brief 串口数据接受完成中断
 *
 * @param huart 串口句柄
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        if (huart->RxEventType != HAL_UART_RXEVENT_TC) {
            memset(uart1_rx_buffer, 0, sizeof(uart1_rx_buffer));
            return;
        }

        uart1_rx_complete = true;
    }
}

void USART1_IRQHandler(void) { HAL_UART_IRQHandler(&huart1); }
