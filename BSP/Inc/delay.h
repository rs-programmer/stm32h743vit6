#ifndef __DELAY_H__
#define __DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/**
 * @brief Get cycle count.
 *
 * @return uint32_t Cycle count.
 */
uint32_t HAL_GetCycleCount(void);

/**
 * @brief Delay in microseconds.
 *
 * @param us Delay in microseconds.
 * @note us max : 41943 = 2^24 / SystemCoreClock
 */
void MX_Delay_us(uint32_t us);

/**
 * @brief Get SysTick value.
 *
 * @return uint32_t SysTick value.
 */
uint32_t MX_SysTick_GetVal(void);

#ifdef __cplusplus
}
#endif
#endif
