#ifndef __SD_H__
#define __SD_H__

#include "main.h"

#define SD_BLOCK_SIZE 512
#define SD_BLOCK_COUNT 1
#define SD_BUFFER_SIZE (SD_BLOCK_SIZE * SD_BLOCK_COUNT)

#define SD_WAIT_TRANSFER()                                                                         \
    do {                                                                                           \
        while (HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER) {                               \
            HAL_Delay(100);                                                                        \
        }                                                                                          \
    } while (0);

extern SD_HandleTypeDef hsd1;

extern __RAM_DMA uint8_t hsd1_buffer0[SD_BUFFER_SIZE];
extern __RAM_DMA uint8_t hsd1_buffer1[SD_BUFFER_SIZE];

HAL_StatusTypeDef MX_SD_Init(void);

void MX_SD_Test(void);

#endif
