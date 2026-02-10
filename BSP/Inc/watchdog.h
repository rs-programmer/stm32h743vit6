#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include "main.h"

#define WATCHDOG_WWDG_RELOAD 127
#define WATCHDOG_IWDG_RELOAD 0xFFF

#define wwdg_feed() HAL_WWDG_Refresh(&hwwdg1)
#define iwdg_feed() HAL_IWDG_Refresh(&hiwdg1)

extern WWDG_HandleTypeDef hwwdg1;
extern IWDG_HandleTypeDef hiwdg1;

/**
 * @brief Initialize WWDG
 *
 * @return HAL_StatusTypeDef HAL_OK if successful
 */
HAL_StatusTypeDef MX_WWDG_Init(void);

/**
 * @brief Initialize IWDG
 *
 * @return HAL_StatusTypeDef HAL_OK if successful
 */
HAL_StatusTypeDef MX_IWDG_Init(void);

#endif
