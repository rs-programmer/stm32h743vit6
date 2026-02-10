#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/**
 * @brief Initialize SysTick.
 *
 */
void HAL_SysTick_Init(void);

#ifdef __cplusplus
}
#endif

#endif
