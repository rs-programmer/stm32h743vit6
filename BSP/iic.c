#include "iic.h"
#include "rng.h"
#include "stm32h743xx.h"
#include "uart.h"

DMA_HandleTypeDef hi2c1_txdma = {0};
DMA_HandleTypeDef hi2c1_rxdma = {0};
I2C_HandleTypeDef hi2c1 = {0};

bool i2c1_tx_complete = false;
bool i2c1_rx_complete = false;

__RAMD2_DMA uint8_t i2c1_tx_buffer[I2C1_TXBUFFER_SIZE] = {0};
__RAMD2_DMA uint8_t i2c1_rx_buffer[I2C1_RXBUFFER_SIZE] = {0};

HAL_StatusTypeDef MX_I2C1_Init(void) {
    HAL_StatusTypeDef ret = HAL_OK;

    /* CSI: 4 MHZ */
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C1CLKSOURCE_D2PCLK1;
    ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    if (ret != HAL_OK) {
        Error_Handler();
    }
    __HAL_RCC_I2C1_CLK_ENABLE();

    /**
     * PB6     ------> I2C1_SCL
     * PB7     ------> I2C1_SDA
     */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* DMA2 配置 */
    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_LINKDMA(&hi2c1, hdmatx, hi2c1_txdma);
    __HAL_LINKDMA(&hi2c1, hdmarx, hi2c1_rxdma);

    /* DMA TX */
    hi2c1_txdma.Instance = DMA2_Stream5;
    hi2c1_txdma.Init.Request = DMA_REQUEST_I2C1_TX;
    hi2c1_txdma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hi2c1_txdma.Init.PeriphInc = DMA_PINC_DISABLE;
    hi2c1_txdma.Init.MemInc = DMA_MINC_ENABLE;
    hi2c1_txdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hi2c1_txdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hi2c1_txdma.Init.Mode = DMA_NORMAL;
    hi2c1_txdma.Init.Priority = DMA_PRIORITY_MEDIUM;
    hi2c1_txdma.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hi2c1_txdma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    hi2c1_txdma.Init.MemBurst = DMA_MBURST_INC4;
    hi2c1_txdma.Init.PeriphBurst = DMA_PBURST_SINGLE;
    ret = HAL_DMA_DeInit(&hi2c1_txdma);
    if (ret != HAL_OK) {
        Error_Handler();
    }
    ret = HAL_DMA_Init(&hi2c1_txdma);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* DMA RX */
    hi2c1_rxdma.Instance = DMA2_Stream4;
    hi2c1_rxdma.Init.Request = DMA_REQUEST_I2C1_RX;
    hi2c1_rxdma.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hi2c1_rxdma.Init.PeriphInc = DMA_PINC_DISABLE;
    hi2c1_rxdma.Init.MemInc = DMA_MINC_ENABLE;
    hi2c1_rxdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hi2c1_rxdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hi2c1_rxdma.Init.Mode = DMA_NORMAL;
    hi2c1_rxdma.Init.Priority = DMA_PRIORITY_MEDIUM;
    hi2c1_rxdma.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hi2c1_rxdma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    hi2c1_rxdma.Init.MemBurst = DMA_MBURST_INC4;
    hi2c1_rxdma.Init.PeriphBurst = DMA_PBURST_SINGLE;
    ret = HAL_DMA_DeInit(&hi2c1_rxdma);
    if (ret != HAL_OK) {
        Error_Handler();
    }
    ret = HAL_DMA_Init(&hi2c1_rxdma);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* GPIO FM+ 适配 1MHZ */
    HAL_I2CEx_EnableFastModePlus(
        I2C_FASTMODEPLUS_PB6 | I2C_FASTMODEPLUS_PB7 | I2C_FASTMODEPLUS_I2C1);

    hi2c1.Instance = I2C1;
    /**
     * 输入时钟源：100MHZ (D2PCLK1) 需要开启 FM+
     * 分频系数：4 + 1 = 5, F_i2c = 100MHZ / 5 = 20MHZ
     * F_clk = F_i2c / (scll + sclh)
     */
    // hi2c1.Init.Timing = 0x00420F13;
    hi2c1.Init.Timing = 0x90000404;
    hi2c1.Init.OwnAddress1 = I2C1_SLAVE_ADDR;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    /**
     * 主模式下必须为0，从模式下可以开启
     * 从机时钟线延长功能，在主机读取数据时不用开启（才有中断和DMA方式）
     * 否则从机可能会干扰主机的时序，导致出现仲裁失败的情况（需要增大时序）
     */
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    /* 默认使能 (I2C_CR2_AUTOEND | I2C_CR2_NACK) */
    ret = HAL_I2C_Init(&hi2c1);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    ret = HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    ret = HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* I2C1 Event IRQ */
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    /* I2C1 Error IRQ */
    // HAL_NVIC_SetPriority(I2C1_ER_IRQn, 14, 0);
    // HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
    /* DMA TX */
    HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
    /* DMA RX */
    HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);

    return ret;
}

void I2C1_EV_IRQHandler(void) { HAL_I2C_EV_IRQHandler(&hi2c1); }
void DMA2_Stream5_IRQHandler(void) { HAL_DMA_IRQHandler(&hi2c1_txdma); }
void DMA2_Stream4_IRQHandler(void) { HAL_DMA_IRQHandler(&hi2c1_rxdma); }

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    /* 主机使用中断模式发送数据完成 也可也是片段发送完成 */
    i2c1_tx_complete = true;
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    /* 主机接受数据完成 也可以是片段接受完成 */
    i2c1_rx_complete = true;
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    /* 从机发送数据完成 也可以是片段发送完成 */
    i2c1_tx_complete = true;
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    /* 从机接受数据完成 也可以是片段接受完成 */
    i2c1_rx_complete = true;
}

void MX_I2C1_Master_Transmit(void) {
    HAL_StatusTypeDef ret = HAL_OK;
    uint32_t random32bit = 0;
    uint32_t sum = 0;
    bool tx_req = false;

    while (1) {
        HAL_Delay(1000);
        sum = 0;

        if (tx_req == false) {
            tx_req = true;
            ret = MX_RNG_RandFull(i2c1_tx_buffer, I2C1_TXBUFFER_SIZE);
            if (ret != HAL_OK) {
                Error_Handler();
            }

            // ret = HAL_I2C_Master_Transmit_IT(
            //     &hi2c1, I2C1_SLAVE_ADDR, i2c1_tx_buffer, I2C1_TXBUFFER_SIZE);
            SCB_CleanDCache_by_Addr(i2c1_tx_buffer, I2C1_TXBUFFER_SIZE);
            ret = HAL_I2C_Master_Transmit_DMA(
                &hi2c1, I2C1_SLAVE_ADDR, i2c1_tx_buffer, I2C1_TXBUFFER_SIZE);
            if ((ret != HAL_OK) && (ret != HAL_BUSY)) {
                Error_Handler();
            }
        }

        if (i2c1_tx_complete == true) {
            i2c1_tx_complete = false;
            tx_req = false;
            SCB_InvalidateDCache_by_Addr(i2c1_tx_buffer, I2C1_TXBUFFER_SIZE);
            for (int i = 0; i < I2C1_TXBUFFER_SIZE; i++) {
                sum += i2c1_tx_buffer[i];
            }
            uart_debug("master send sum: 0x%x\n", sum);
        }

        // for (int i = 0; i < I2C1_TXBUFFER_SIZE; i++) {
        //     ret = rand(random32bit);
        //     if (ret != HAL_OK) {
        //         Error_Handler();
        //     }
        //     i2c1_tx_buffer[i] = (uint8_t)(random32bit & 0xFF);
        //     sum += i2c1_tx_buffer[i];
        // }
        /* 自带了状态机 */
        // ret = HAL_I2C_Master_Transmit(
        //     &hi2c1, I2C1_SLAVE_ADDR, i2c1_tx_buffer, I2C1_TXBUFFER_SIZE, HAL_MAX_DELAY);
        // if (ret == HAL_OK) {
        //     uart_debug("master send sum: 0x%x\n", sum);
        // }
    }
}

void MX_I2C1_Master_Receive(void) {
    HAL_StatusTypeDef ret = HAL_OK;
    uint32_t sum = 0;
    bool rx_req = false;

    while (1) {
        HAL_Delay(1000);
        sum = 0;
        // ret = HAL_I2C_Master_Receive(
        //     &hi2c1, I2C1_SLAVE_ADDR, i2c1_rx_buffer, I2C1_RXBUFFER_SIZE, HAL_MAX_DELAY);
        // for (int i = 0; i < I2C1_RXBUFFER_SIZE; i++) {
        //     sum += i2c1_rx_buffer[i];
        // }
        if (rx_req == false) {
            rx_req = true;
            // ret = HAL_I2C_Master_Receive_IT(
            //     &hi2c1, I2C1_SLAVE_ADDR, i2c1_rx_buffer, I2C1_RXBUFFER_SIZE);
            ret = HAL_I2C_Master_Receive_DMA(
                &hi2c1, I2C1_SLAVE_ADDR, i2c1_rx_buffer, I2C1_RXBUFFER_SIZE);
            if ((ret != HAL_OK) && (ret != HAL_BUSY)) {
                Error_Handler();
            }
        }

        if (i2c1_rx_complete == true) {
            i2c1_rx_complete = false;
            rx_req = false;
            /* 处理接收到的数据 */
            SCB_InvalidateDCache_by_Addr(i2c1_rx_buffer, I2C1_RXBUFFER_SIZE);
            for (int i = 0; i < I2C1_RXBUFFER_SIZE; i++) {
                sum += i2c1_rx_buffer[i];
            }
            uart_debug("master receive sum: 0x%x\n", sum);
        }
        // uart_debug("sum: 0x%x\n", sum);
    }
}

void MX_I2C1_Slave_Receive(void) {
    HAL_StatusTypeDef ret = HAL_OK;
    uint32_t sum = 0;
    bool rx_req = false;

    while (1) {
        sum = 0;
        if (rx_req == false) {
            rx_req = true;
            // ret = HAL_I2C_Slave_Receive_IT(&hi2c1, i2c1_rx_buffer, I2C1_RXBUFFER_SIZE);
            ret = HAL_I2C_Slave_Receive_DMA(&hi2c1, i2c1_rx_buffer, I2C1_RXBUFFER_SIZE);
            if ((ret != HAL_OK) && (ret != HAL_BUSY)) {
                Error_Handler();
            }
        }
        if (i2c1_rx_complete == true) {
            i2c1_rx_complete = false;
            rx_req = false;
            /* 处理接收到的数据 */
            SCB_InvalidateDCache_by_Addr(i2c1_rx_buffer, I2C1_RXBUFFER_SIZE);
            for (int i = 0; i < I2C1_RXBUFFER_SIZE; i++) {
                sum += i2c1_rx_buffer[i];
            }
            uart_debug("slave receive sum: 0x%x\n", sum);
        }

        // ret = HAL_I2C_Slave_Receive(&hi2c1, i2c1_rx_buffer, I2C1_RXBUFFER_SIZE, HAL_MAX_DELAY);
        // ret = HAL_I2C_Slave_Receive_IT(&hi2c1, i2c1_rx_buffer, I2C1_RXBUFFER_SIZE);
        // if ((ret != HAL_OK) && (ret != HAL_BUSY)) {
        //     Error_Handler();
        // }
        // for (int i = 0; i < I2C1_RXBUFFER_SIZE; i++) {
        //     sum += i2c1_rx_buffer[i];
        // }
        // uart_debug("sum = 0x%x\n", sum);
    }
}

void MX_I2C1_Slave_Transmit(void) {
    HAL_StatusTypeDef ret = HAL_OK;
    uint32_t random32bit = 0;
    uint32_t sum = 0;
    bool tx_req = false;

    while (1) {
        sum = 0;

        if (tx_req == false) {
            tx_req = true;
            ret = MX_RNG_RandFull(i2c1_tx_buffer, I2C1_TXBUFFER_SIZE);
            if (ret != HAL_OK) {
                Error_Handler();
            }

            // ret = HAL_I2C_Slave_Transmit_IT(&hi2c1, i2c1_tx_buffer, I2C1_TXBUFFER_SIZE);
            SCB_CleanDCache_by_Addr(i2c1_tx_buffer, I2C1_TXBUFFER_SIZE);
            ret = HAL_I2C_Slave_Transmit_DMA(&hi2c1, i2c1_tx_buffer, I2C1_TXBUFFER_SIZE);
            if (ret != HAL_OK) {
                Error_Handler();
            }
        }

        if (i2c1_tx_complete == true) {
            i2c1_tx_complete = false;
            tx_req = false;
            SCB_InvalidateDCache_by_Addr(i2c1_tx_buffer, I2C1_TXBUFFER_SIZE);
            for (int i = 0; i < I2C1_TXBUFFER_SIZE; i++) {
                sum += i2c1_tx_buffer[i];
            }
            uart_debug("slave send sum: 0x%x\n", sum);
        }
        // ret = HAL_I2C_Slave_Transmit(&hi2c1, i2c1_tx_buffer, I2C1_TXBUFFER_SIZE, HAL_MAX_DELAY);
        // for (int i = 0; i < I2C1_TXBUFFER_SIZE; i++) {
        //     sum += i2c1_tx_buffer[i];
        // }
        // if (ret == HAL_OK) {
        //     uart_debug("slave send sum: 0x%x\n", sum);
        // }
    }
}