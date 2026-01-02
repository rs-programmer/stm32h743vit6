#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "stm32h7xx_hal.h"

/**
 * HSE: 25 MHZ
 * PLL2_P: ((HSE / PLL2M) * PLL2N) / PLL2P
 * PLL2_Q: ((HSE / PLL2M) * PLL2N) / PLL2Q
 * PLL2_R: ((HSE / PLL2M) * PLL2N) / PLL2R
 * PLL2RGE: HSE / PLL2M
 */
#define RCC_PLL2_DEFAULT                                                                           \
    {                                                                                              \
        .PLL2M = 5,                                                                                \
        .PLL2N = 160,                                                                              \
        .PLL2P = 2,                                                                                \
        .PLL2Q = 2,                                                                                \
        .PLL2R = 20,                                                                               \
        .PLL2RGE = RCC_PLL2VCIRANGE_2,                                                             \
        .PLL2VCOSEL = RCC_PLL2VCOWIDE,                                                             \
        .PLL2FRACN = 0,                                                                            \
    }

/**
 * HSE: 25 MHZ
 * PLL3_P: ((HSE / PLL3M) * PLL3N) / PLL3P
 * PLL3_Q: ((HSE / PLL3M) * PLL3N) / PLL3Q
 * PLL3_R: ((HSE / PLL3M) * PLL3N) / PLL3R
 * PLL3RGE: HSE / PLL3M
 */
#define RCC_PLL3_DEFAULT                                                                           \
    {                                                                                              \
        .PLL3M = 5,                                                                                \
        .PLL3N = 160,                                                                              \
        .PLL3P = 2,                                                                                \
        .PLL3Q = 2,                                                                                \
        .PLL3R = 20,                                                                               \
        .PLL3RGE = RCC_PLL3VCIRANGE_2,                                                             \
        .PLL3VCOSEL = RCC_PLL3VCOWIDE,                                                             \
        .PLL3FRACN = 0,                                                                            \
    }

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
