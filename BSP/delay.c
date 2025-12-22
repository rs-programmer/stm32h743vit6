#include "delay.h"

#define DWT_CR *(__IO uint32_t *)0xE0001000
#define DWT_CYCCNT *(__IO uint32_t *)0xE0001004
#define DEM_CR *(__IO uint32_t *)0xE000EDFC

#define DEM_CR_TRCENA (1 << 24)
#define DWT_CR_CYCCNTENA (1 << 0)

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
    /* 使能DWT外设 */
    DEM_CR |= (uint32_t)DEM_CR_TRCENA;
    /* DWT CYCCNT寄存器计数清0 */
    DWT_CYCCNT = (uint32_t)0u;
    /* 使能Cortex-M DWT CYCCNT寄存器 */
    DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
    return HAL_OK;
}

uint32_t HAL_GetCycleCount(void) { return DWT_CYCCNT; }

uint32_t HAL_GetTick(void) {
    return ((uint32_t)DWT_CYCCNT / (SystemCoreClock / 1000));
}

void HAL_Delay_us(uint32_t us) {
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;

    ticks = us * (SystemCoreClock / 1000000); /* 需要的节拍数 */
    tcnt = 0;
    told = (uint32_t)HAL_GetCycleCount(); /* 刚进入时的计数器值 */

    while (1) {
        tnow = (uint32_t)HAL_GetCycleCount();
        if (tnow != told) {
            /* 32位计数器是递增计数器 */
            if (tnow > told) {
                tcnt += tnow - told;
            }
            /* 重新装载 */
            else {
                tcnt += UINT32_MAX - told + tnow;
            }

            told = tnow;

            /* 时间超过/等于要延迟的时间,则退出 */
            if (tcnt >= ticks) {
                break;
            }
        }
    }
}

void HAL_Delay(uint32_t Delay) { HAL_Delay_us(Delay * 1000); }
