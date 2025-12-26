#include "uart.h"

DMA_HandleTypeDef huart1_txdma = {0};
DMA_HandleTypeDef huart1_rxdma = {0};
UART_HandleTypeDef huart1 = {0};

__attribute__((aligned(32))) __IO uint8_t uart1_rx_buffer[15];
bool uart1_rx_complete = false;

HAL_StatusTypeDef MX_UART1_Init(void) {
    HAL_StatusTypeDef ret = HAL_OK;

    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART16;
    PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_PCLK2;
    ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    if (ret != HAL_OK) {
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
    __HAL_RCC_DMA2_CLK_ENABLE();

    __HAL_LINKDMA(&huart1, hdmatx, huart1_txdma);
    __HAL_LINKDMA(&huart1, hdmarx, huart1_rxdma);

    /* Tx DMA配置 */
    huart1_txdma.Instance = DMA2_Stream7;
    huart1_txdma.Init.Request = DMA_REQUEST_USART1_TX;
    huart1_txdma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    huart1_txdma.Init.PeriphInc = DMA_PINC_DISABLE;
    huart1_txdma.Init.MemInc = DMA_MINC_ENABLE;
    huart1_txdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    huart1_txdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    huart1_txdma.Init.Mode = DMA_NORMAL;
    huart1_txdma.Init.Priority = DMA_PRIORITY_MEDIUM;
    huart1_txdma.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    huart1_txdma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    huart1_txdma.Init.MemBurst = DMA_MBURST_INC4;
    huart1_txdma.Init.PeriphBurst = DMA_PBURST_SINGLE;
    ret = HAL_DMA_DeInit(&huart1_txdma);
    if (ret != HAL_OK) {
        Error_Handler();
    }
    ret = HAL_DMA_Init(&huart1_txdma);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* Rx DMA配置 */
    huart1_rxdma.Instance = DMA2_Stream6;
    huart1_rxdma.Init.Request = DMA_REQUEST_USART1_RX;
    huart1_rxdma.Init.Direction = DMA_PERIPH_TO_MEMORY;
    huart1_rxdma.Init.PeriphInc = DMA_PINC_DISABLE;
    huart1_rxdma.Init.MemInc = DMA_MINC_ENABLE;
    huart1_rxdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    huart1_rxdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    huart1_rxdma.Init.Mode = DMA_NORMAL;
    huart1_rxdma.Init.Priority = DMA_PRIORITY_MEDIUM;
    huart1_rxdma.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    huart1_rxdma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    huart1_rxdma.Init.MemBurst = DMA_MBURST_INC4;
    huart1_rxdma.Init.PeriphBurst = DMA_PBURST_SINGLE;
    ret = HAL_DMA_DeInit(&huart1_rxdma);
    if (ret != HAL_OK) {
        Error_Handler();
    }
    ret = HAL_DMA_Init(&huart1_rxdma);
    if (ret != HAL_OK) {
        Error_Handler();
    }

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
    ret = HAL_UART_Init(&huart1);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* FIFO 空闲个数大于等于 1/8 时触发中断 */
    ret = HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* FIFO 接收数据个数大于等于 1/8 时触发中断 */
    ret = HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    ret = HAL_UARTEx_EnableFifoMode(&huart1);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* 开启中断 */
    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
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
    /** 非空闲方式接受完成，可采用中断，DMA方式
     *  DMA有传输过半中断，传输完成中断
     */
    if (huart->Instance == USART1) {
        if (huart->RxEventType == HAL_UART_RXEVENT_HT) {
            return;
        }

        uart1_rx_complete = true;
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    /** 空闲方式接受完成，可采用中断，DMA方式
     *  DMA有传输过半中断，传输完成中断
     */
    if (huart->Instance == USART1) {
        if (huart->RxEventType == HAL_UART_RXEVENT_HT) {
            return;
        }
        uart1_rx_buffer[Size] = '\0';
        uart1_rx_complete = true;
    }
}

void USART1_IRQHandler(void) { HAL_UART_IRQHandler(&huart1); }

void DMA2_Stream7_IRQHandler(void) { HAL_DMA_IRQHandler(&huart1_txdma); }
void DMA2_Stream6_IRQHandler(void) { HAL_DMA_IRQHandler(&huart1_rxdma); }
