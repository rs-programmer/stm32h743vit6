#include "sd.h"
#include "main.h"
#include "stm32h743xx.h"
#include "stm32h7xx_hal_def.h"
#include "stm32h7xx_hal_sd.h"
#include "stm32h7xx_hal_sd_ex.h"

// SD_HandleTypeDef hsd1 = {0};

bool hsd1_tx_complete = false;
bool hsd1_rx_complate = false;

__RAM_DMA uint8_t hsd1_buffer0[SD_BUFFER_SIZE] = {0};
__RAM_DMA uint8_t hsd1_buffer1[SD_BLOCK_SIZE] = {0};

HAL_StatusTypeDef MX_SD_Init(void) {

    HAL_StatusTypeDef ret = HAL_OK;

    /* 240MHZ */
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
    PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        Error_Handler();
    }

    __HAL_RCC_SDMMC1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**
     * @brief 初始化SDMMC1 GPIO 引脚
     * PC8     ------> SDMMC1_D0
     * PC9     ------> SDMMC1_D1
     * PC10    ------> SDMMC1_D2
     * PC11    ------> SDMMC1_D3
     * PC12    ------> SDMMC1_CK
     * PD2     ------> SDMMC1_CMD
     */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    hsd1.Instance = SDMMC1;
    hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING; // 大多数SD卡都支持上升沿
    hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_ENABLE;
    hsd1.Init.BusWide = SDMMC_BUS_WIDE_4B;
    hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;
    hsd1.Init.ClockDiv = 3;
    ret = HAL_SD_Init(&hsd1);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* 双缓冲 IDMA */
    // ret = HAL_SDEx_ConfigDMAMultiBuffer(&hsd1, hsd1_buffer0, hsd1_buffer1, SD_BLOCK_COUNT);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(SDMMC1_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(SDMMC1_IRQn);

    return ret;
}

void SDMMC1_IRQHandler(void) { HAL_SD_IRQHandler(&hsd1); }

#if 1
// void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd) {
//     /* IT / IDMA 传输完成 */
//     if (hsd->Instance == SDMMC1) {
//         hsd1_tx_complete = true;
//     }
// }

// void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd) {
//     /* IT / IDMA 传输完成 */
//     if (hsd->Instance == SDMMC1) {
//         hsd1_rx_complate = true;
//     }
// }

// void HAL_SDEx_Read_DMADoubleBuf0CpltCallback(SD_HandleTypeDef *hsd) {}
// void HAL_SDEx_Read_DMADoubleBuf1CpltCallback(SD_HandleTypeDef *hsd) {}

// void HAL_SDEx_Write_DMADoubleBuf0CpltCallback(SD_HandleTypeDef *hsd) {}

// void HAL_SDEx_Write_DMADoubleBuf1CpltCallback(SD_HandleTypeDef *hsd) {}

void MX_SD_Test(void) {
    HAL_StatusTypeDef ret = HAL_OK;
    hsd1_tx_complete = false;
    hsd1_rx_complate = false;

    /* 检测SD卡就绪 */
    SD_WAIT_TRANSFER();

    ret = HAL_SD_Erase(&hsd1, 0, 10);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    memset(hsd1_buffer0, 0x11, SD_BUFFER_SIZE);
    memset(hsd1_buffer1, 0x22, SD_BUFFER_SIZE);

    SCB_CleanDCache_by_Addr((uint32_t *)hsd1_buffer0, SD_BUFFER_SIZE);
    SCB_CleanDCache_by_Addr((uint32_t *)hsd1_buffer1, SD_BUFFER_SIZE);

    /* 检测SD卡就绪 */
    SD_WAIT_TRANSFER();

    // ret = HAL_SD_WriteBlocks(&hsd1, hsd1_buffer0, 0, SD_BLOCK_COUNT, HAL_MAX_DELAY);
    // ret = HAL_SD_WriteBlocks_IT(&hsd1, hsd1_buffer0, 0, SD_BLOCK_COUNT);
    ret = HAL_SD_WriteBlocks_DMA(&hsd1, hsd1_buffer0, 0, SD_BLOCK_COUNT);
    // ret = HAL_SDEx_WriteBlocksDMAMultiBuffer(&hsd1, 0, SD_BLOCK_COUNT);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* 检测SD卡就绪 */
    // SD_WAIT_TRANSFER();
    // while (!hsd1_tx_complete) {
    HAL_Delay(1000);
    // }

    // ret = HAL_SD_ReadBlocks(&hsd1, hsd1_buffer1, 0, SD_BLOCK_COUNT, HAL_MAX_DELAY);
    // ret = HAL_SD_ReadBlocks_IT(&hsd1, hsd1_buffer1, 0, SD_BLOCK_COUNT);
    ret = HAL_SD_ReadBlocks_DMA(&hsd1, hsd1_buffer1, 0, SD_BLOCK_COUNT);
    // HAL_SDEx_ChangeDMABuffer(&hsd1, SD_DMA_BUFFER0, hsd1_buffer1);
    // ret = HAL_SDEx_ReadBlocksDMAMultiBuffer(&hsd1, 0, SD_BLOCK_COUNT);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    // while (!hsd1_rx_complate) {
    HAL_Delay(1000);
    // }

    SCB_InvalidateDCache_by_Addr(hsd1_buffer0, SD_BUFFER_SIZE);
    SCB_InvalidateDCache_by_Addr(hsd1_buffer1, SD_BUFFER_SIZE);
    for (int i = 0; i < SD_BUFFER_SIZE; i++) {
        if (hsd1_buffer0[i] != hsd1_buffer1[i]) {
            Error_Handler();
        }
    }
}
#endif
