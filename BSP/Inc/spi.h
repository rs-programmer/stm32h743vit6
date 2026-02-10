#ifndef __SPI_H__
#define __SPI_H__

#include "main.h"

#define SPI1_TXBUFFER_SIZE 4096
#define SPI1_RXBUFFER_SIZE 4096

extern SPI_HandleTypeDef hspi1;

extern bool spi1_tx_complete;
extern bool spi1_rx_complete;

extern __RAMD2_DMA __IO uint8_t spi1_tx_buf[SPI1_TXBUFFER_SIZE];
extern __RAMD2_DMA __IO uint8_t spi1_rx_buf[SPI1_RXBUFFER_SIZE];

/**
 * @brief Initialize SPI1
 *
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef MX_SPI1_Init(uint32_t mode);

void MX_SPI1_Master_Test(void);
void MX_SPI1_Slave_Test(void);

#endif
