#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "stm32h7xx_hal.h"

#define __ALIGN(x) __attribute__((aligned(x)))

#define __RAM_DATA_ALIGN(x) __attribute__((section(".ram.data"), aligned(x)))
#define __RAM_FUNC_ALIGN(x) __attribute__((section(".ram.func"), aligned(x)))
#define __RAM_BSS_ALIGN(x) __attribute__((section(".ram.bss"), aligned(x)))
#define __RAM_DATA_NOT_CACHED(x) __attribute__((section(".ram_not_cached.data"), aligned(x)))
#define __RAM_BSS_NOT_CACHED(x) __attribute__((section(".ram_not_cached.bss"), aligned(x)))
#define __RAM_DMA __RAM_BSS_ALIGN(32)

#define __RAMD2_BSS_ALIGN(x) __attribute__((section(".ramd2.bss"), aligned(x)))
#define __RAMD2_DMA __RAMD2_BSS_ALIGN(32)

#define __RAMD3_BSS_ALIGN(x) __attribute__((section(".ramd3.bss"), aligned(x)))
#define __RAMD3_DMA __RAMD3_BSS_ALIGN(32)
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

#define uapi_min(a, b) ((a) < (b) ? (a) : (b))
#define uapi_max(a, b) ((a) > (b) ? (a) : (b))

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
