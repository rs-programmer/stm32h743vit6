#ifndef __IIC_H__
#define __IIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define I2C1_SLAVE_ADDR (0x78)

#define I2C1_TXBUFFER_SIZE 1024
#define I2C1_RXBUFFER_SIZE 1024

extern I2C_HandleTypeDef hi2c1;

extern bool i2c1_tx_complete;
extern bool i2c1_rx_complete;

extern __RAMD2_DMA uint8_t i2c1_tx_buffer[I2C1_TXBUFFER_SIZE];
extern __RAMD2_DMA uint8_t i2c1_rx_buffer[I2C1_RXBUFFER_SIZE];

/**
 * @brief Initialize IIC1
 * 1. RELOAD模式启动时，AUTOEND无效，并且在RELOAD模式下，主机将不会发送停止位，只会延长SCL
 * 2. AUTOEND=0，软件确定发送START还是STOP
 * 3. AUTOEND=1，主机发送完数据后，会自动发送STOP位
 *
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef MX_I2C1_Init(void);

/* TEST CODE */
void MX_I2C1_Master_Transmit(void);
void MX_I2C1_Master_Receive(void);
void MX_I2C1_Slave_Receive(void);
void MX_I2C1_Slave_Transmit(void);

#ifdef __cplusplus
}
#endif
#endif