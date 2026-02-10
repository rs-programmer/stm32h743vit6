#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define LED0_ON()     // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET)
#define LED0_OFF()    // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET)
#define LED0_TOGGLE() // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2)

#define LED1_ON() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define LED1_OFF() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)
#define LED1_TOGGLE() HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3)

/**
 * @brief Initialize GPIO.
 *
 * This function initializes GPIO with the specified pin configuration.
 *
 * @param None
 * @retval None
 */
void MX_GPIO_Init(void);

#ifdef __cplusplus
}
#endif
#endif