#ifndef __ADC_H__
#define __ADC_H__

#include "main.h"
#include "stm32h7xx_hal_def.h"

#define ADC1_REG_NUM 2
#define ADC1_INJ_NUM 2

extern ADC_HandleTypeDef hadc1;

extern bool adc1_reg_complete;
extern bool adc1_inj_complete;

extern __RAMD2_DMA uint16_t adc1_reg_value[ADC1_REG_NUM];
extern __RAMD2_DMA uint16_t adc1_inj_value[ADC1_INJ_NUM];

/**
 * @brief ADC1 Init
 *
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef MX_ADC1_Init(void);

void MX_ADC1_Test(void);

#endif
