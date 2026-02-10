#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stm32h7xx_hal.h"

#define UART1_TXBUFFER_SIZE 1024
#define UART1_RXBUFFER_SIZE 1024

extern UART_HandleTypeDef huart1;

extern bool uart1_tx_complete;
extern bool uart1_rx_complete;

extern __RAMD2_DMA uint8_t uart1_tx_buffer[UART1_TXBUFFER_SIZE];
extern __RAMD2_DMA uint8_t uart1_rx_buffer[UART1_RXBUFFER_SIZE];

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

#if (defined(USE_FULL_ASSERT) && (USE_FULL_ASSERT > 0U))
void uart_debug(const char *fmt, ...);
#else
#define uart_debug(fmt, ...)
#endif

HAL_StatusTypeDef MX_UART1_TXIT(uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef MX_UART1_RXIT(uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef MX_UART1_TXDMA(uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef MX_UART1_RXDMA(uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef MX_UART1_RXIDLE(uint8_t *pData, uint16_t Size, uint32_t *RxLen);
HAL_StatusTypeDef MX_UART1_RXIDLE_IT(uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef MX_UART1_RXIDLE_DMA(uint8_t *pData, uint16_t Size);

void MX_UART1_Test(void);

#ifdef __cplusplus
}
#endif

#endif /* __UART_H__ */
