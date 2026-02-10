#ifndef __TIM_H__
#define __TIM_H__

#include "main.h"

#define TIM15_UPDMA_NUM 10

#define HAL_SuspendTick() __HAL_TIM_DISABLE_IT(&htim6, TIM_IT_UPDATE)
#define HAL_ResumeTick() __HAL_TIM_ENABLE_IT(&htim6, TIM_IT_UPDATE)

extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim15;

extern __RAMD2_DMA uint16_t htim15_updma_buf[TIM15_UPDMA_NUM];

/**
 * @brief TIM6, TIM7 BaseTime Init
 *
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef MX_TIM6_Init(void);

/**
 * @brief TIM15 BaseTime Init
 *
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef MX_TIM15_Init(void);

/**
 * @brief Get TIM6 Counter Value, Unit: US
 *
 * @return uint32_t
 */
uint32_t MX_TIM6_GetUS(void);

HAL_StatusTypeDef MX_TIM15_Test(void);

#endif
