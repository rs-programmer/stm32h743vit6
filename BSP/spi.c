#include "spi.h"
#include "delay.h"
#include "rng.h"
#include "stm32h743xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_spi.h"
#include "uart.h"

DMA_HandleTypeDef hspi1_txdma = {0};
DMA_HandleTypeDef hspi1_rxdma = {0};
SPI_HandleTypeDef hspi1 = {0};

bool spi1_tx_complete = false;
bool spi1_rx_complete = false;

__RAMD2_DMA __IO uint8_t spi1_tx_buf[SPI1_TXBUFFER_SIZE] = {0};
__RAMD2_DMA __IO uint8_t spi1_rx_buf[SPI1_RXBUFFER_SIZE] = {0};

HAL_StatusTypeDef MX_SPI1_Init(uint32_t Mode) {
    HAL_StatusTypeDef ret = HAL_OK;

    assert_param(IS_SPI_MODE(Mode));

    /* PLL1_Q: 240MHZ */
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI1;
    PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        Error_Handler();
    }

    __HAL_RCC_SPI1_CLK_ENABLE();

    /**
     * PA4 ------> SPI1_NSS
     * PA5 ------> SPI1_SCK
     * PA6 ------> SPI1_MISO
     * PD7 ------> SPI1_MOSI
     */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* DMA2 配置 */
    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_LINKDMA(&hspi1, hdmatx, hspi1_txdma);
    __HAL_LINKDMA(&hspi1, hdmarx, hspi1_rxdma);

    /* DMA TX */
    hspi1_txdma.Instance = DMA2_Stream3;
    hspi1_txdma.Init.Request = DMA_REQUEST_SPI1_TX;
    hspi1_txdma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hspi1_txdma.Init.PeriphInc = DMA_PINC_DISABLE;
    hspi1_txdma.Init.MemInc = DMA_MINC_ENABLE;
    hspi1_txdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hspi1_txdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hspi1_txdma.Init.Mode = DMA_NORMAL;
    hspi1_txdma.Init.Priority = DMA_PRIORITY_MEDIUM;
    hspi1_txdma.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hspi1_txdma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    hspi1_txdma.Init.MemBurst = DMA_MBURST_INC4;
    hspi1_txdma.Init.PeriphBurst = DMA_PBURST_SINGLE;
    ret = HAL_DMA_DeInit(&hspi1_txdma);
    if (ret != HAL_OK) {
        Error_Handler();
    }
    ret = HAL_DMA_Init(&hspi1_txdma);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* DMA RX */
    hspi1_rxdma.Instance = DMA2_Stream2;
    hspi1_rxdma.Init.Request = DMA_REQUEST_SPI1_RX;
    hspi1_rxdma.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hspi1_rxdma.Init.PeriphInc = DMA_PINC_DISABLE;
    hspi1_rxdma.Init.MemInc = DMA_MINC_ENABLE;
    hspi1_rxdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hspi1_rxdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hspi1_rxdma.Init.Mode = DMA_NORMAL;
    hspi1_rxdma.Init.Priority = DMA_PRIORITY_MEDIUM;
    hspi1_rxdma.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hspi1_rxdma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    hspi1_rxdma.Init.MemBurst = DMA_MBURST_INC4;
    hspi1_rxdma.Init.PeriphBurst = DMA_PBURST_SINGLE;
    ret = HAL_DMA_DeInit(&hspi1_rxdma);
    if (ret != HAL_OK) {
        Error_Handler();
    }
    ret = HAL_DMA_Init(&hspi1_rxdma);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    hspi1.Instance = SPI1;
    hspi1.Init.Mode = Mode;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    if (Mode == SPI_MODE_MASTER) {
        hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
    } else {
        hspi1.Init.NSS = SPI_NSS_HARD_INPUT;
    }
    /* 高速SPI需要共地 高速基本可达60 MHZ */
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; // 禁用CRC
    hspi1.Init.CRCPolynomial = 0x107;
    hspi1.Init.CRCLength = SPI_CRC_LENGTH_8BIT;
    hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE; // 禁用NSS脉冲模式
    hspi1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
    hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_04DATA;
    hspi1.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi1.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE; // 首次SS到数据发送的空闲时间
    hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE; // 数据间隔空闲时间
    hspi1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
    hspi1.Init.IOSwap = SPI_IO_SWAP_DISABLE;
    ret = HAL_SPI_Init(&hspi1);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* SPI1 Event IRQ */
    HAL_NVIC_SetPriority(SPI1_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

    return ret;
}

void SPI1_IRQHandler(void) { HAL_SPI_IRQHandler(&hspi1); }
void DMA2_Stream3_IRQHandler(void) { HAL_DMA_IRQHandler(&hspi1_txdma); }
void DMA2_Stream2_IRQHandler(void) { HAL_DMA_IRQHandler(&hspi1_rxdma); }

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) { spi1_tx_complete = true; }

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) { spi1_rx_complete = true; }

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
    spi1_tx_complete = true;
    spi1_rx_complete = true;
}

void MX_SPI1_Master_Test(void) {
    HAL_StatusTypeDef ret = HAL_OK;
    uint32_t sum1 = 0, sum2 = 0;
    bool rx_req = false, tx_req = false;

    while (1) {
        HAL_Delay(500);
        sum1 = 0;
        sum2 = 0;

        if (!rx_req) {
            rx_req = true;
            spi1_rx_complete = false;

            ret = MX_RNG_RandFull(spi1_tx_buf, SPI1_TXBUFFER_SIZE);
            if (ret != HAL_OK) {
                Error_Handler();
            }

            // ret = HAL_SPI_Transmit(&hspi1, spi1_tx_buf, SPI1_TXBUFFER_SIZE, HAL_MAX_DELAY);
            // ret = HAL_SPI_Receive(&hspi1, spi1_rx_buf, SPI1_RXBUFFER_SIZE, HAL_MAX_DELAY);
            // ret = HAL_SPI_TransmitReceive(
            //     &hspi1, spi1_tx_buf, spi1_rx_buf, SPI1_TXBUFFER_SIZE, HAL_MAX_DELAY);
            // ret = HAL_SPI_Transmit_IT(&hspi1, spi1_tx_buf, SPI1_TXBUFFER_SIZE);
            // ret = HAL_SPI_Receive_IT(&hspi1, spi1_rx_buf, SPI1_RXBUFFER_SIZE);
            // ret = HAL_SPI_TransmitReceive_IT(&hspi1, spi1_tx_buf, spi1_rx_buf,
            // SPI1_TXBUFFER_SIZE);
            // SCB_CleanDCache_by_Addr(spi1_tx_buf, SPI1_TXBUFFER_SIZE);
            // ret = HAL_SPI_Transmit_DMA(&hspi1, spi1_tx_buf, SPI1_TXBUFFER_SIZE);
            // ret = HAL_SPI_Receive_DMA(&hspi1, spi1_rx_buf, SPI1_RXBUFFER_SIZE);

            SCB_CleanDCache_by_Addr(spi1_tx_buf, SPI1_TXBUFFER_SIZE);
            ret = HAL_SPI_TransmitReceive_DMA(&hspi1, spi1_tx_buf, spi1_rx_buf, SPI1_TXBUFFER_SIZE);
            if (ret != HAL_OK) {
                Error_Handler();
            }
        }

        if (spi1_rx_complete) {
            rx_req = false;
            spi1_rx_complete = false;

            SCB_InvalidateDCache_by_Addr(spi1_rx_buf, SPI1_RXBUFFER_SIZE);
            for (int i = 0; i < SPI1_RXBUFFER_SIZE; i++) {
                sum1 += spi1_rx_buf[i];
                sum2 += spi1_tx_buf[i];
            }

            uart_debug("master rx: 0x%x, tx: 0x%x\n", sum1, sum2);
        }
    }
}

void MX_SPI1_Slave_Test(void) {
    HAL_StatusTypeDef ret = HAL_OK;
    uint32_t sum1 = 0, sum2 = 0;
    bool rx_req = false, tx_req = false;

    while (1) {
        sum1 = 0;
        sum2 = 0;

        if (!tx_req) {
            tx_req = true;
            spi1_tx_complete = false;

            ret = MX_RNG_RandFull(spi1_tx_buf, SPI1_TXBUFFER_SIZE);
            if (ret != HAL_OK) {
                Error_Handler();
            }

            // ret = HAL_SPI_Receive(&hspi1, spi1_rx_buf, SPI1_RXBUFFER_SIZE, HAL_MAX_DELAY);
            // ret = HAL_SPI_Transmit(&hspi1, spi1_tx_buf, SPI1_TXBUFFER_SIZE, HAL_MAX_DELAY);
            // ret = HAL_SPI_TransmitReceive(
            //     &hspi1, spi1_tx_buf, spi1_rx_buf, SPI1_TXBUFFER_SIZE, HAL_MAX_DELAY);
            // ret = HAL_SPI_Receive_IT(&hspi1, spi1_rx_buf, SPI1_RXBUFFER_SIZE);
            // ret = HAL_SPI_Transmit_IT(&hspi1, spi1_tx_buf, SPI1_TXBUFFER_SIZE);
            // ret = HAL_SPI_TransmitReceive_IT(&hspi1, spi1_tx_buf, spi1_rx_buf,
            // SPI1_TXBUFFER_SIZE);
            // ret = HAL_SPI_Receive_DMA(&hspi1, spi1_rx_buf, SPI1_RXBUFFER_SIZE);

            // SCB_CleanDCache_by_Addr(spi1_tx_buf, SPI1_TXBUFFER_SIZE);
            // ret = HAL_SPI_Transmit_DMA(&hspi1, spi1_tx_buf, SPI1_TXBUFFER_SIZE);

            SCB_CleanDCache_by_Addr(spi1_tx_buf, SPI1_TXBUFFER_SIZE);
            ret = HAL_SPI_TransmitReceive_DMA(&hspi1, spi1_tx_buf, spi1_rx_buf, SPI1_TXBUFFER_SIZE);
            if (ret != HAL_OK) {
                Error_Handler();
            }
        }

        if (spi1_tx_complete) {
            tx_req = false;
            spi1_tx_complete = false;

            SCB_InvalidateDCache_by_Addr(spi1_rx_buf, SPI1_RXBUFFER_SIZE);
            for (int i = 0; i < SPI1_TXBUFFER_SIZE; i++) {
                sum1 += spi1_rx_buf[i];
                sum2 += spi1_tx_buf[i];
            }

            uart_debug("slave rx: 0x%x, tx: 0x%x\n", sum1, sum2);
        }
    }
}