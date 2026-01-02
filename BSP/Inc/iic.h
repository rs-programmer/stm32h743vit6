#ifndef __IIC_H__
#define __IIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/**
 * @brief Initialize IIC1
 * 
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef MX_I2C1_Init(void);

#ifdef __cplusplus
}
#endif
#endif