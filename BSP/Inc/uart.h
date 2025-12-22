#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define printf_it(str, len) HAL_UART_Transmit_IT(&huart1, (uint8_t *)str, len)
#define scanf_it(str, len) HAL_UART_Receive_IT(&huart1, (uint8_t *)str, len)

extern UART_HandleTypeDef huart1;
extern uint8_t uart1_rx_buffer[128];
extern bool uart1_rx_complete;

/**
 * @brief Initialize UART1.
 *
 * This function initializes UART1 with the specified baud rate, word length,
 * stop bits, parity, mode, hardware flow control, and over-sampling.
 *
 * @param None
 * @retval HAL_StatusTypeDef HAL_OK if initialization is successful,
 *         otherwise an error code.
 */
HAL_StatusTypeDef MX_UART1_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __UART_H__ */
