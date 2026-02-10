#ifndef __EXTI_H__
#define __EXTI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define MX_KEY_Init() MX_EXTI13_Init()

/**
 * @brief Initialize EXTI0.
 * @note WAKE_UP pin is GPIOA_PIN_0.
 *
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef MX_EXTI0_Init(void);

/**
 * @brief Initialize EXTI1.
 * @note KEY1 pin is GPIOA_PIN_1.
 *
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef MX_EXTI1_Init(void);

/**
 * @brief Initialize EXTI13.
 * @note KEY2 pin is GPIOB_PIN_13.
 *
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef MX_EXTI13_Init(void);

#ifdef __cplusplus
}
#endif

#endif
