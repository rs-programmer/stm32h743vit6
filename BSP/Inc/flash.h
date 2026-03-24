#ifndef __FLASH_H__
#define __FLASH_H__

#include "main.h"
#include "stm32h7xx_hal_def.h"

#define FLASH_WORD_SIZE 32
#define FLASH_SECTOR_SIZE 0x20000

void MX_FLASH_OB_Config(void);

HAL_StatusTypeDef MX_FLASH_Sector_Program(
    uint8_t Banks, uint8_t Sector, uint32_t SrcAddress, uint32_t SrcSize);

#endif
