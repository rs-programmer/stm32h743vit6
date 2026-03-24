#include "flash.h"
#include "stm32h7xx_hal_conf.h"
#include "stm32h7xx_hal_def.h"
#include "stm32h7xx_hal_flash.h"
#include "stm32h7xx_hal_flash_ex.h"
#include "uart.h"
#include <stdint.h>
#include <string.h>

static void FLASH_OB_Printf(FLASH_OBProgramInitTypeDef *OBConfig) {
    /* print OBConfig info */
    uart_debug("OptionType: %p\n", OBConfig->OptionType);
    uart_debug("WRPState: %p, WRPSector: %p\n", OBConfig->WRPState, OBConfig->WRPSector);
    uart_debug("RDPLevel: %p\n", OBConfig->RDPLevel);
    uart_debug("BORLevel: %p\n", OBConfig->BORLevel);
    uart_debug("USERType: %p, USERConfig: %p\n", OBConfig->USERType, OBConfig->USERConfig);
    uart_debug("Banks: %p\n", OBConfig->Banks);
    uart_debug("PCROPConfig: %p, PCROPStartAddr: %p, PCROPEndAddr: %p\n", OBConfig->PCROPConfig,
        OBConfig->PCROPStartAddr, OBConfig->PCROPEndAddr);
    /* 设置Boot地址时需要主动配置 BootConfig */
    uart_debug("BootConfig: %p, BootAddr0: %p, BootAddr1: %p\n", OBConfig->BootConfig,
        OBConfig->BootAddr0, OBConfig->BootAddr1);
    uart_debug("SecureAreaConfig: %p, SecureAreaStartAddr: %p, SecureAreaEndAddr: %p\n",
        OBConfig->SecureAreaConfig, OBConfig->SecureAreaStartAddr, OBConfig->SecureAreaEndAddr);
}

static HAL_StatusTypeDef FLASH_Sector_Program_Check(
    uint8_t Banks, uint8_t Sector, uint32_t DstAddress, uint32_t SrcAddress, uint32_t SrcSize) {
    uint32_t DstEndAddress = DstAddress + SrcSize - 1;
    uint32_t SrcEndAddress = SrcAddress + SrcSize - 1;

    if ((DstAddress == NULL) || (SrcSize == 0) || (SrcSize & (FLASH_WORD_SIZE - 1) != 0)) {
        return HAL_ERROR;
    }

    if (!(IS_FLASH_PROGRAM_ADDRESS_BANK1(DstAddress) ||
            IS_FLASH_PROGRAM_ADDRESS_BANK2(SrcAddress))) {
        return HAL_ERROR;
    }

    if (!(IS_FLASH_PROGRAM_ADDRESS_BANK1(DstEndAddress) ||
            IS_FLASH_PROGRAM_ADDRESS_BANK2(SrcEndAddress))) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

void MX_FLASH_OB_Config(void) {
    FLASH_OBProgramInitTypeDef OBConfig;
    OBConfig.Banks = FLASH_BANK_1;
    HAL_FLASHEx_OBGetConfig(&OBConfig);
    FLASH_OB_Printf(&OBConfig);

    OBConfig.Banks = FLASH_BANK_2;
    HAL_FLASHEx_OBGetConfig(&OBConfig);
    FLASH_OB_Printf(&OBConfig);
}

HAL_StatusTypeDef MX_FLASH_Sector_Program(
    uint8_t Banks, uint8_t Sector, uint32_t SrcAddress, uint32_t SrcSize) {
    HAL_StatusTypeDef ret = HAL_OK;
    uint32_t DstAddress = Sector * FLASH_SECTOR_SIZE;
    uint8_t SectorCnt = (SrcSize + FLASH_SECTOR_SIZE - 1) / FLASH_SECTOR_SIZE;

    if (Banks == FLASH_BANK_1) {
        DstAddress += FLASH_BANK1_BASE;
    } else {
        DstAddress += FLASH_BANK2_BASE;
    }

    if (FLASH_Sector_Program_Check(Banks, Sector, DstAddress, SrcAddress, SrcSize) != HAL_OK) {
        return HAL_ERROR;
    }

    if (HAL_FLASH_Unlock() != HAL_OK) {
        return HAL_ERROR;
    }

    FLASH_EraseInitTypeDef EraseInit = {0};
    uint32_t SectorError = 0;
    EraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    if (Sector + SectorCnt > FLASH_SECTOR_TOTAL) {
        EraseInit.Sector = Sector;
        EraseInit.Banks = Banks;
        EraseInit.NbSectors = FLASH_SECTOR_TOTAL - Sector;
        ret |= HAL_FLASHEx_Erase(&EraseInit, &SectorError);

        /* 跨区擦除时后半段只能是Bank2 */
        EraseInit.Sector = FLASH_SECTOR_0;
        EraseInit.Banks = FLASH_BANK_2;
        EraseInit.NbSectors = SectorCnt + Sector - FLASH_SECTOR_TOTAL;
        ret |= HAL_FLASHEx_Erase(&EraseInit, &SectorError);
    } else {
        EraseInit.Sector = Sector;
        EraseInit.Banks = Banks;
        EraseInit.NbSectors = SectorCnt;
        ret |= HAL_FLASHEx_Erase(&EraseInit, &SectorError);
    }

    if (ret != HAL_OK) {
        ret |= HAL_FLASH_Lock();
        return ret;
    }

    uint32_t ProgramCnt = (SrcSize + FLASH_WORD_SIZE - 1) / FLASH_WORD_SIZE;
    for (uint32_t i = 0; i < ProgramCnt; i++) {
        uint32_t dst = DstAddress + i * FLASH_WORD_SIZE;
        uint32_t src = SrcAddress + i * FLASH_WORD_SIZE;
        ret = HAL_FLASHEx_ProgramWord(&dst, &src);
        if (ret != HAL_OK) {
            ret |= HAL_FLASH_Lock();
            return ret;
        }
    }

    ret |= HAL_FLASH_Lock();
    return ret;
}