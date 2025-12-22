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
 * @note us max : 10737418 = 2^32 / SystemCoreClock
 */
void HAL_Delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif
#endif
