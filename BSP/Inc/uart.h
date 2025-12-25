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

#define printf_it(str, len)                                                                        \
    do {                                                                                           \
        __HAL_UART_SEND_REQ(&huart1, UART_TXDATA_FLUSH_REQUEST);                                   \
        HAL_UART_Transmit_IT(&huart1, (uint8_t *)str, len);                                        \
    } while (0)

#define scanf_it(str, len)                                                                         \
    do {                                                                                           \
        __HAL_UART_SEND_REQ(&huart1, UART_RXDATA_FLUSH_REQUEST);                                   \
        HAL_UART_Receive_IT(&huart1, (uint8_t *)str, len);                                         \
    } while (0)

#define printf_dma(str, len)                                                                       \
    do {                                                                                           \
        __HAL_UART_SEND_REQ(&huart1, UART_TXDATA_FLUSH_REQUEST);                                   \
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)str, len);                                       \
    } while (0)

#define scanf_dma(str, len)                                                                        \
    do {                                                                                           \
        __HAL_UART_SEND_REQ(&huart1, UART_RXDATA_FLUSH_REQUEST);                                   \
        HAL_UART_Receive_DMA(&huart1, (uint8_t *)str, len);                                        \
    } while (0)

#define scanf_idle(str, len, rxlen)                                                                \
    do {                                                                                           \
        __HAL_UART_SEND_REQ(&huart1, UART_RXDATA_FLUSH_REQUEST);                                   \
        HAL_UARTEx_ReceiveToIdle(&huart1, (uint8_t *)str, len, rxlen, HAL_MAX_DELAY);              \
    } while (0)

#define scanf_idleit(str, len)                                                                     \
    do {                                                                                           \
        __HAL_UART_SEND_REQ(&huart1, UART_RXDATA_FLUSH_REQUEST);                                   \
        HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t *)str, len);                                 \
    } while (0);

#define scanf_idledma(str, len)                                                                    \
    do {                                                                                           \
        __HAL_UART_SEND_REQ(&huart1, UART_RXDATA_FLUSH_REQUEST);                                   \
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)str, len);                                \
    } while (0);

extern UART_HandleTypeDef huart1;
extern __attribute__((aligned(64))) __IO uint8_t uart1_rx_buffer[15];
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
