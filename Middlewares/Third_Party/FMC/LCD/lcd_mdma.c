#include "cmsis_os2.h"
#include "lcd.h"
#include "stm32h743xx.h"
#include "stm32h7xx_hal_def.h"
#include <stdint.h>

#if (FMC_LCD_MODE_MDMA == 1)
#include "lcd.h"
#include "lcdfont.h"
#include "stdlib.h"

#include "cmsis_os.h"
#include "fmc.h"
#include "lcd_ex.c"

#define LCD_DMA_BUFFER_ROW 10
#define LCD_DMA_BUFFER_COL 320
#define LCD_DMA_BUFFER_SIZE (LCD_DMA_BUFFER_ROW * LCD_DMA_BUFFER_COL * sizeof(uint16_t))
#define LCD_DMA_TRANSFER_SIZE 0xFFFF

#define LCD_SEMAPHORE_ACQUIRE() osSemaphoreAcquire(lcd_mutex, osWaitForever)
#define LCD_SEMAPHORE_RELEASE() osSemaphoreRelease(lcd_mutex)

typedef HAL_StatusTypeDef (*lcd_func_t)(lcd_msg_t *msg);

SRAM_HandleTypeDef hsram;
MDMA_HandleTypeDef hsram_mdma = {0};

_lcd_dev lcddev;

osSemaphoreId_t lcd_mutex = NULL;

/* 320 * 480 * 16 的屏幕采用分块刷新的方式 */
__RAM_BSS_ALIGN(2) uint16_t lcd_dma_buffer[LCD_DMA_BUFFER_ROW][LCD_DMA_BUFFER_COL];

void LCD_WriteData(volatile uint16_t data) {
    data = data;
    LCD->LCD_RAM = data;
}

void LCD_WriteReg(volatile uint16_t regno) {
    regno = regno;
    LCD->LCD_REG = regno;
}

void LCD_Write(uint16_t regno, uint16_t data) {
    LCD->LCD_REG = regno; /* 写入要写的寄存器序号 */
    LCD->LCD_RAM = data;  /* 写入数据 */
}

static uint16_t LCD_ReadData(void) {
    volatile uint16_t ram; /* 防止被优化 */
    ram = LCD->LCD_RAM;
    return ram;
}

static void LCD_Delay(uint32_t i) {
    while (i--)
        ; /* 使用AC6时空循环可能被优化,可使用while(1) __asm volatile(""); */
}

void LCD_WritePrepare(void) { LCD->LCD_REG = lcddev.wramcmd; }

void LCD_SetCursor(uint16_t x, uint16_t y) {
    if (lcddev.id == 0X5510) /* 5510设置坐标 */
    {
        LCD_WriteReg(lcddev.setxcmd);
        LCD_WriteData(x >> 8);
        LCD_WriteReg(lcddev.setxcmd + 1);
        LCD_WriteData(x & 0XFF);
        LCD_WriteReg(lcddev.setycmd);
        LCD_WriteData(y >> 8);
        LCD_WriteReg(lcddev.setycmd + 1);
        LCD_WriteData(y & 0XFF);
    } else /* 5310/7789/7796设置坐标 */
    {
        LCD_WriteReg(lcddev.setxcmd);
        LCD_WriteData(x >> 8);
        LCD_WriteData(x & 0XFF);
        LCD_WriteReg(lcddev.setycmd);
        LCD_WriteData(y >> 8);
        LCD_WriteData(y & 0XFF);
    }
}

uint32_t LCD_ReadPoint(uint16_t x, uint16_t y) {
    uint16_t r = 0, g = 0, b = 0;

    if (x >= lcddev.width || y >= lcddev.height) {
        return 0; /* 超过了范围,直接返回 */
    }

    LCD_SetCursor(x, y); /* 设置坐标 */

    if (lcddev.id == 0x5510) {
        LCD_WriteReg(0x2E00); /* 5510 发送读GRAM指令 */
    } else {
        LCD_WriteReg(0x2E); /* 其他IC(7796/5310/7789)发送读GRAM指令 */
    }

    r = LCD_ReadData(); /* 假读(dummy read) */

    LCD_Delay(2);
    r = LCD_ReadData(); /* 实际坐标颜色 */

    if (lcddev.id == 0x7796) /* 7796 一次读取一个像素值 */
    {
        return r;
    }

    /* 5310/5510/7789 要分2次读出 */
    LCD_Delay(2);
    b = LCD_ReadData();
    g = r & 0XFF; /* 对于 5310/5510/7789, 第一次读取的是RG的值,R在前,G在后,各占8位 */
    g <<= 8;

    return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11)); /* 5310/5510/7789 需要公式转换一下 */
}

void LCD_DisplayOn(void) {
    if (lcddev.id == 0X5510) /* 5510开启显示指令 */
    {
        LCD_WriteReg(0X2900); /* 开启显示 */
    } else                    /* 5310/7789/7796 等发送开启显示指令 */
    {
        LCD_WriteReg(0X29); /* 开启显示 */
    }
}

void LCD_DisplayOff(void) {
    if (lcddev.id == 0X5510) /* 5510关闭显示指令 */
    {
        LCD_WriteReg(0X2800); /* 关闭显示 */
    } else                    /* 5310/7789/7796 等发送开启显示指令 */
    {
        LCD_WriteReg(0X28); /* 关闭显示 */
    }
}

void LCD_ScanDir(uint8_t dir) {
    uint16_t regval = 0;
    uint16_t dirreg = 0;
    uint16_t temp;

    /* 横屏时，IC改变扫描方向！竖屏时, IC不改变扫描方向 */
    if (lcddev.dir == 1) {
        switch (dir) /* 方向转换 */
        {
        case 0:
            dir = 6;
            break;

        case 1:
            dir = 7;
            break;

        case 2:
            dir = 4;
            break;

        case 3:
            dir = 5;
            break;

        case 4:
            dir = 1;
            break;

        case 5:
            dir = 0;
            break;

        case 6:
            dir = 3;
            break;

        case 7:
            dir = 2;
            break;
        }
    }

    /* 根据扫描方式 设置 0X36/0X3600 寄存器 bit 5,6,7 位的值 */
    switch (dir) {
    case L2R_U2D: /* 从左到右,从上到下 */
        regval |= (0 << 7) | (0 << 6) | (0 << 5);
        break;

    case L2R_D2U: /* 从左到右,从下到上 */
        regval |= (1 << 7) | (0 << 6) | (0 << 5);
        break;

    case R2L_U2D: /* 从右到左,从上到下 */
        regval |= (0 << 7) | (1 << 6) | (0 << 5);
        break;

    case R2L_D2U: /* 从右到左,从下到上 */
        regval |= (1 << 7) | (1 << 6) | (0 << 5);
        break;

    case U2D_L2R: /* 从上到下,从左到右 */
        regval |= (0 << 7) | (0 << 6) | (1 << 5);
        break;

    case U2D_R2L: /* 从上到下,从右到左 */
        regval |= (0 << 7) | (1 << 6) | (1 << 5);
        break;

    case D2U_L2R: /* 从下到上,从左到右 */
        regval |= (1 << 7) | (0 << 6) | (1 << 5);
        break;

    case D2U_R2L: /* 从下到上,从右到左 */
        regval |= (1 << 7) | (1 << 6) | (1 << 5);
        break;
    }

    dirreg = 0X36; /* 对绝大部分驱动IC, 由0X36寄存器控制 */

    if (lcddev.id == 0X5510) {
        dirreg = 0X3600; /* 对于5510, 和其他驱动ic的寄存器有差异 */
    }

    /* 7789 & 7796 要设置BGR位 */
    if (lcddev.id == 0X7789 || lcddev.id == 0X7796) {
        regval |= 0X08;
    }

    LCD_Write(dirreg, regval);

    if (regval & 0X20) {
        if (lcddev.width < lcddev.height) /* 交换X,Y */
        {
            temp = lcddev.width;
            lcddev.width = lcddev.height;
            lcddev.height = temp;
        }
    } else {
        if (lcddev.width > lcddev.height) /* 交换X,Y */
        {
            temp = lcddev.width;
            lcddev.width = lcddev.height;
            lcddev.height = temp;
        }
    }

    /* 设置显示区域(开窗)大小 */
    if (lcddev.id == 0X5510) {
        LCD_WriteReg(lcddev.setxcmd);
        LCD_WriteData(0);
        LCD_WriteReg(lcddev.setxcmd + 1);
        LCD_WriteData(0);
        LCD_WriteReg(lcddev.setxcmd + 2);
        LCD_WriteData((lcddev.width - 1) >> 8);
        LCD_WriteReg(lcddev.setxcmd + 3);
        LCD_WriteData((lcddev.width - 1) & 0XFF);
        LCD_WriteReg(lcddev.setycmd);
        LCD_WriteData(0);
        LCD_WriteReg(lcddev.setycmd + 1);
        LCD_WriteData(0);
        LCD_WriteReg(lcddev.setycmd + 2);
        LCD_WriteData((lcddev.height - 1) >> 8);
        LCD_WriteReg(lcddev.setycmd + 3);
        LCD_WriteData((lcddev.height - 1) & 0XFF);
    } else {
        LCD_WriteReg(lcddev.setxcmd);
        LCD_WriteData(0);
        LCD_WriteData(0);
        LCD_WriteData((lcddev.width - 1) >> 8);
        LCD_WriteData((lcddev.width - 1) & 0XFF);
        LCD_WriteReg(lcddev.setycmd);
        LCD_WriteData(0);
        LCD_WriteData(0);
        LCD_WriteData((lcddev.height - 1) >> 8);
        LCD_WriteData((lcddev.height - 1) & 0XFF);
    }
}

void LCD_DrawPoint(uint16_t x, uint16_t y, uint32_t color) {
    LCD_SetCursor(x, y); /* 设置光标位置 */
    LCD_WritePrepare();  /* 开始写入GRAM */
    LCD->LCD_RAM = color;
}

void LCD_DisplayDir(uint8_t dir) {
    lcddev.dir = dir; /* 竖屏/横屏 */

    if (dir == 0) /* 竖屏 */
    {
        lcddev.width = 240;
        lcddev.height = 320;

        if (lcddev.id == 0x5510) {
            lcddev.wramcmd = 0X2C00; /* 设置写入GRAM的指令 */
            lcddev.setxcmd = 0X2A00; /* 设置写X坐标指令 */
            lcddev.setycmd = 0X2B00; /* 设置写Y坐标指令 */
            lcddev.width = 480;      /* 设置宽度480 */
            lcddev.height = 800;     /* 设置高度800 */
        } else                       /* 其他IC, 包括: 5310/7789/7796等IC */
        {
            lcddev.wramcmd = 0X2C;
            lcddev.setxcmd = 0X2A;
            lcddev.setycmd = 0X2B;
        }

        if (lcddev.id == 0X5310 || lcddev.id == 0X7796) /* 如果是5310/7796 则表示是 320*480分辨率 */
        {
            lcddev.width = 320;
            lcddev.height = 480;
        }
    } else /* 横屏 */
    {
        lcddev.width = 320;  /* 默认宽度 */
        lcddev.height = 240; /* 默认高度 */

        if (lcddev.id == 0x5510) {
            lcddev.wramcmd = 0X2C00; /* 设置写入GRAM的指令 */
            lcddev.setxcmd = 0X2A00; /* 设置写X坐标指令 */
            lcddev.setycmd = 0X2B00; /* 设置写Y坐标指令 */
            lcddev.width = 800;      /* 设置宽度800 */
            lcddev.height = 480;     /* 设置高度480 */
        } else                       /* 其他IC, 包括: 5310/7789/7796等IC */
        {
            lcddev.wramcmd = 0X2C;
            lcddev.setxcmd = 0X2A;
            lcddev.setycmd = 0X2B;
        }

        if (lcddev.id == 0X5310 || lcddev.id == 0X7796) /* 如果是5310/7796 则表示是 320*480分辨率 */
        {
            lcddev.width = 480;
            lcddev.height = 320;
        }
    }

    LCD_ScanDir(DFT_SCAN_DIR); /* 默认扫描方向 */
}

void LCD_SetWindow(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height) {
    uint16_t twidth, theight;
    twidth = sx + width - 1;
    theight = sy + height - 1;

    if (lcddev.id == 0X5510) /* 5510设置窗口 */
    {
        LCD_WriteReg(lcddev.setxcmd);
        LCD_WriteData(sx >> 8);
        LCD_WriteReg(lcddev.setxcmd + 1);
        LCD_WriteData(sx & 0XFF);
        LCD_WriteReg(lcddev.setxcmd + 2);
        LCD_WriteData(twidth >> 8);
        LCD_WriteReg(lcddev.setxcmd + 3);
        LCD_WriteData(twidth & 0XFF);
        LCD_WriteReg(lcddev.setycmd);
        LCD_WriteData(sy >> 8);
        LCD_WriteReg(lcddev.setycmd + 1);
        LCD_WriteData(sy & 0XFF);
        LCD_WriteReg(lcddev.setycmd + 2);
        LCD_WriteData(theight >> 8);
        LCD_WriteReg(lcddev.setycmd + 3);
        LCD_WriteData(theight & 0XFF);
    } else /* 5310/7789/7796设置窗口 */
    {
        LCD_WriteReg(lcddev.setxcmd);
        LCD_WriteData(sx >> 8);
        LCD_WriteData(sx & 0XFF);
        LCD_WriteData(twidth >> 8);
        LCD_WriteData(twidth & 0XFF);
        LCD_WriteReg(lcddev.setycmd);
        LCD_WriteData(sy >> 8);
        LCD_WriteData(sy & 0XFF);
        LCD_WriteData(theight >> 8);
        LCD_WriteData(theight & 0XFF);
    }
}

void HAL_SRAM_DMA_XferCpltCallback(MDMA_HandleTypeDef *hmdma) { LCD_SEMAPHORE_RELEASE(); }

HAL_StatusTypeDef LCD_MDMA_Init(int32_t SourceBlockAddressOffset) {
    HAL_StatusTypeDef ret = HAL_OK;

    __HAL_RCC_MDMA_CLK_ENABLE();
    __HAL_LINKDMA(&hsram, hmdma, hsram_mdma);

    /* MDMA 配置 */
    hsram_mdma.Instance = MDMA_Channel0;
    hsram_mdma.Init.Request = MDMA_REQUEST_SW;
    hsram_mdma.Init.TransferTriggerMode = MDMA_REPEAT_BLOCK_TRANSFER; /* 重复传输模式 */
    hsram_mdma.Init.Priority = MDMA_PRIORITY_HIGH;
    hsram_mdma.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
    hsram_mdma.Init.SourceInc = MDMA_SRC_INC_HALFWORD;
    hsram_mdma.Init.DestinationInc = MDMA_DEST_INC_DISABLE;
    hsram_mdma.Init.SourceDataSize = MDMA_SRC_DATASIZE_HALFWORD;
    hsram_mdma.Init.DestDataSize = MDMA_DEST_DATASIZE_HALFWORD;
    hsram_mdma.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
    hsram_mdma.Init.BufferTransferLength = 128; /* 缓冲区大小 = 数据宽度 * 突发节拍数 */
    hsram_mdma.Init.SourceBurst = MDMA_SOURCE_BURST_64BEATS;
    hsram_mdma.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
    hsram_mdma.Init.SourceBlockAddressOffset = SourceBlockAddressOffset;
    hsram_mdma.Init.DestBlockAddressOffset = 0;

    ret = HAL_MDMA_DeInit(&hsram_mdma);
    if (ret != HAL_OK) {
        Error_Handler();
    }
    ret = HAL_MDMA_Init(&hsram_mdma);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(MDMA_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(MDMA_IRQn);
    return ret;
}

void MDMA_IRQHandler(void) { HAL_MDMA_IRQHandler(&hsram_mdma); }

static void SRAM_DMACplt(MDMA_HandleTypeDef *hmdma) {
    /* Derogation MISRAC2012-Rule-11.5 */
    SRAM_HandleTypeDef *hsram = (SRAM_HandleTypeDef *)(hmdma->Parent);

    /* Disable the MDMA channel */
    __HAL_MDMA_DISABLE(hmdma);

    /* Update the SRAM controller state */
    hsram->State = HAL_SRAM_STATE_READY;

#if (USE_HAL_SRAM_REGISTER_CALLBACKS == 1)
    hsram->DmaXferCpltCallback(hmdma);
#else
    HAL_SRAM_DMA_XferCpltCallback(hmdma);
#endif /* USE_HAL_SRAM_REGISTER_CALLBACKS */
}

static void SRAM_DMAError(MDMA_HandleTypeDef *hmdma) {
    /* Derogation MISRAC2012-Rule-11.5 */
    SRAM_HandleTypeDef *hsram = (SRAM_HandleTypeDef *)(hmdma->Parent);

    /* Disable the MDMA channel */
    __HAL_MDMA_DISABLE(hmdma);

    /* Update the SRAM controller state */
    hsram->State = HAL_SRAM_STATE_ERROR;

#if (USE_HAL_SRAM_REGISTER_CALLBACKS == 1)
    hsram->DmaXferErrorCallback(hmdma);
#else
    HAL_SRAM_DMA_XferErrorCallback(hmdma);
#endif /* USE_HAL_SRAM_REGISTER_CALLBACKS */
}

HAL_StatusTypeDef MX_LCD_Write_DMA(SRAM_HandleTypeDef *hsram, uint32_t *pAddress,
    uint32_t *pSrcBuffer, uint32_t BufferSize, uint32_t BlockCount) {
    HAL_StatusTypeDef status;

    /* Check the SRAM controller state */
    if (hsram->State == HAL_SRAM_STATE_READY) {
        /* Process Locked */
        __HAL_LOCK(hsram);

        /* Update the SRAM controller state */
        hsram->State = HAL_SRAM_STATE_BUSY;

        /* Configure DMA user callbacks */
        hsram->hmdma->XferCpltCallback = SRAM_DMACplt;
        hsram->hmdma->XferErrorCallback = SRAM_DMAError;

        /* Enable the DMA Stream */
        status = HAL_MDMA_Start_IT(
            hsram->hmdma, (uint32_t)pSrcBuffer, (uint32_t)pAddress, BufferSize, BlockCount);

        /* Process unlocked */
        __HAL_UNLOCK(hsram);
    } else {
        status = HAL_ERROR;
    }

    return status;
}

HAL_StatusTypeDef LCD_Transmit_DMA(
    int32_t SourceBlockAddressOffset, uint32_t BlockCount) {
    HAL_StatusTypeDef ret = HAL_OK;

    ret = LCD_MDMA_Init(SourceBlockAddressOffset);
    assert_param(ret == HAL_OK);

    SCB_CleanDCache_by_Addr((uint32_t *)lcd_dma_buffer, sizeof(lcd_dma_buffer));
    ret = MX_LCD_Write_DMA(&hsram, (uint32_t *)LCD_RAM_ADDR, (uint32_t *)lcd_dma_buffer,
        LCD_DMA_BUFFER_SIZE, BlockCount);
    assert_param(ret == HAL_OK);

    LCD_SEMAPHORE_ACQUIRE();
    return ret;
}

void MX_LCD_Init(void) {
    GPIO_InitTypeDef gpio_init_struct;
    FMC_NORSRAM_TimingTypeDef fmc_read_handle;
    FMC_NORSRAM_TimingTypeDef fmc_write_handle;

    LCD_CS_GPIO_CLK_ENABLE();  /* LCD_CS脚时钟使能 */
    LCD_WR_GPIO_CLK_ENABLE();  /* LCD_WR脚时钟使能 */
    LCD_RD_GPIO_CLK_ENABLE();  /* LCD_RD脚时钟使能 */
    LCD_RS_GPIO_CLK_ENABLE();  /* LCD_RS脚时钟使能 */
    LCD_BL_GPIO_CLK_ENABLE();  /* LCD_BL脚时钟使能 */
    LCD_RST_GPIO_CLK_ENABLE(); /* LCD_RST脚时钟使能 */

    gpio_init_struct.Pin = LCD_CS_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* 复用推挽 */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* 高速 */
    gpio_init_struct.Alternate = GPIO_AF12_FMC;         /* 复用为FMC */
    HAL_GPIO_Init(LCD_CS_GPIO_PORT, &gpio_init_struct); /* 初始化LCD_CS引脚 */

    gpio_init_struct.Pin = LCD_WR_GPIO_PIN;
    HAL_GPIO_Init(LCD_WR_GPIO_PORT, &gpio_init_struct); /* 初始化LCD_WR引脚 */

    gpio_init_struct.Pin = LCD_RD_GPIO_PIN;
    HAL_GPIO_Init(LCD_RD_GPIO_PORT, &gpio_init_struct); /* 初始化LCD_RD引脚 */

    gpio_init_struct.Pin = LCD_RS_GPIO_PIN;
    HAL_GPIO_Init(LCD_RS_GPIO_PORT, &gpio_init_struct); /* 初始化LCD_RS引脚 */

    gpio_init_struct.Pin = LCD_BL_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;        /* 推挽输出 */
    HAL_GPIO_Init(LCD_BL_GPIO_PORT, &gpio_init_struct); /* LCD_BL引脚模式设置(推挽输出) */

    gpio_init_struct.Pin = LCD_RST_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;         /* 推挽输出 */
    HAL_GPIO_Init(LCD_RST_GPIO_PORT, &gpio_init_struct); /* LCD_RST引脚模式设置(推挽输出) */

    hsram.Instance = FMC_NORSRAM_DEVICE;
    hsram.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
    hsram.Init.NSBank = FMC_NORSRAM_BANK1;                     /* 使用NE1 */
    hsram.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;  /* 地址/数据线不复用 */
    hsram.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;              /* SRAM */
    hsram.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16; /* 16位数据宽度 */
    hsram.Init.BurstAccessMode =
        FMC_BURST_ACCESS_MODE_DISABLE; /* 是否使能突发访问,仅对同步突发存储器有效,此处未用到 */
    hsram.Init.WaitSignalPolarity =
        FMC_WAIT_SIGNAL_POLARITY_LOW; /* 等待信号的极性,仅在突发模式访问下有用 */
    hsram.Init.WaitSignalActive =
        FMC_WAIT_TIMING_BEFORE_WS; /* 存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
                                    */
    hsram.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE; /* 存储器写使能 */
    hsram.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;        /* 等待使能位,此处未用到 */
    hsram.Init.ExtendedMode = FMC_EXTENDED_MODE_ENABLE;     /* 读写使用不同的时序 */
    hsram.Init.AsynchronousWait =
        FMC_ASYNCHRONOUS_WAIT_DISABLE; /* 是否使能同步传输模式下的等待信号,此处未用到 */
    hsram.Init.WriteBurst = FMC_WRITE_BURST_DISABLE; /* 禁止突发写 */
    hsram.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
    hsram.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
    hsram.Init.PageSize = FMC_PAGE_SIZE_NONE;

    /* FMC读时序控制寄存器 */
    fmc_read_handle.AddressSetupTime =
        0x0F; /* 地址建立时间(ADDSET)为15个fmc_ker_ck 1/220M = 4.5ns * 15 = 67.5ns */
    fmc_read_handle.AddressHoldTime = 0x01;
    fmc_read_handle.DataSetupTime =
        0x4E; /* 数据保存时间(DATAST)为78个fmc_ker_ck = 4.5 * 78 = 351ns */
              /* 因为液晶驱动IC的读数据的时候，速度不能太快,尤其是个别奇葩芯片 */
    fmc_read_handle.AccessMode = FMC_ACCESS_MODE_A; /* 模式A */
    fmc_read_handle.CLKDivision = 2;
    fmc_read_handle.DataLatency = 2;
    /* FMC写时序控制寄存器 */
    fmc_write_handle.AddressSetupTime = 0x0F; /* 地址建立时间(ADDSET)为15个fmc_ker_ck = 67.5ns */
    fmc_write_handle.AddressHoldTime = 0x01;
    fmc_write_handle.DataSetupTime = 0x0F; /* 数据保存时间(DATAST)为15个fmc_ker_ck = 67.5ns */
    /* 15个fmc_ker_ck（fmc_ker_ck=220Mhz）,某些液晶驱动IC的写信号脉宽，最少也得50ns。 */
    fmc_write_handle.AccessMode = FMC_ACCESS_MODE_A; /* 模式A */
    HAL_SRAM_Init(&hsram, &fmc_read_handle, &fmc_write_handle);
    delay_ms(50);

    /* LCD复位 */
    LCD_RST(1);
    delay_ms(10);
    LCD_RST(0);
    delay_ms(50);
    LCD_RST(1);
    delay_ms(200);

    /* 尝试7796 ID的读取 */
    LCD_WriteReg(0XD3);
    lcddev.id = LCD_ReadData(); /* dummy read */
    lcddev.id = LCD_ReadData(); /* 读到0X00 */
    lcddev.id = LCD_ReadData(); /* 读取0X77 */
    lcddev.id <<= 8;
    lcddev.id |= LCD_ReadData(); /* 读取0X96 */

    if (lcddev.id != 0X7796) /* 不是7796,尝试看看是不是ST7789 */
    {
        LCD_WriteReg(0X04);
        lcddev.id = LCD_ReadData(); /* dummy read */
        lcddev.id = LCD_ReadData(); /* 读到0X85 */
        lcddev.id = LCD_ReadData(); /* 读取0X85 */
        lcddev.id <<= 8;
        lcddev.id |= LCD_ReadData(); /* 读取0X52 */

        if (lcddev.id == 0X8552) /* 将8552的ID转换成7789 */
        {
            lcddev.id = 0x7789;
        }

        if (lcddev.id != 0x7789) /* 也不是ST7789,尝试是不是NT35310 */
        {
            LCD_WriteReg(0XD4);
            lcddev.id = LCD_ReadData(); /* dummy read */
            lcddev.id = LCD_ReadData(); /* 读回0X01 */
            lcddev.id = LCD_ReadData(); /* 读回0X53 */
            lcddev.id <<= 8;
            lcddev.id |= LCD_ReadData(); /* 这里读回0X10 */

            if (lcddev.id != 0X5310) /* 也不是NT35310,尝试看看是不是NT35510 */
            {
                /* 发送秘钥（厂家提供,照搬即可） */
                LCD_Write(0xF000, 0x0055);
                LCD_Write(0xF001, 0x00AA);
                LCD_Write(0xF002, 0x0052);
                LCD_Write(0xF003, 0x0008);
                LCD_Write(0xF004, 0x0001);

                LCD_WriteReg(0xC500);       /* 读取ID高8位 */
                lcddev.id = LCD_ReadData(); /* 读回0X55 */
                lcddev.id <<= 8;

                LCD_WriteReg(0xC501);        /* 读取ID低8位 */
                lcddev.id |= LCD_ReadData(); /* 读回0X10 */
            }
        }
    }

    /* 特别注意, 如果在main函数里面屏蔽串口1初始化, 则会卡死在printf
     * 里面(卡死在f_putc函数), 所以, 必须初始化串口1, 或者屏蔽掉下面
     * 这行 printf 语句 !!!!!!!
     */
    // printf("LCD ID:%x\r\n", lcddev.id); /* 打印LCD ID */

    if (lcddev.id == 0X7789) {
        lcd_ex_st7789_reginit(); /* 执行ST7789初始化 */
    } else if (lcddev.id == 0x5310) {
        lcd_ex_nt35310_reginit(); /* 执行NT35310初始化 */
    } else if (lcddev.id == 0x7796) {
        lcd_ex_st7796_reginit(); /* 执行ST7796初始化 */
    } else if (lcddev.id == 0x5510) {
        lcd_ex_nt35510_reginit(); /* 执行NT35510初始化 */
    }

    /* 由于不同屏幕的写时序不同，这里的时序可以根据自己的屏幕进行修改
      （若插上长排线对时序也会有影响，需要自己根据情况修改） */
    /* 初始化完成以后,提速 */
    if (lcddev.id == 0x7789) {
        /* 重新配置写时序控制寄存器的时序 */
        fmc_write_handle.AddressSetupTime = 5; /* 地址建立时间(ADDSET)为5个fmc_ker_ck = 22.5 ns */
        fmc_write_handle.DataSetupTime = 5;    /* 数据保存时间(DATAST)为5个fmc_ker_ck = 22.5 ns */
        FMC_NORSRAM_Extended_Timing_Init(
            hsram.Extended, &fmc_write_handle, hsram.Init.NSBank, hsram.Init.ExtendedMode);
    } else if (lcddev.id == 0x5310 || lcddev.id == 0x5510 || lcddev.id == 0x7796) {
        /* 重新配置写时序控制寄存器的时序 */
        fmc_write_handle.AddressSetupTime = 3; /* 地址建立时间(ADDSET)为3个fmc_ker_ck = 13.5 ns */
        fmc_write_handle.DataSetupTime = 3;    /* 数据保存时间(DATAST)为3个fmc_ker_ck = 13.5 ns */
        FMC_NORSRAM_Extended_Timing_Init(
            hsram.Extended, &fmc_write_handle, hsram.Init.NSBank, hsram.Init.ExtendedMode);
    }

    LCD_DisplayDir(0); /* 默认为竖屏 */
    LCD_BL(1);         /* 点亮背光 */
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram) {
    GPIO_InitTypeDef gpio_init_struct;

    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
    PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_D1HCLK;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    __HAL_RCC_FMC_CLK_ENABLE();   /* 使能FMC时钟 */
    __HAL_RCC_GPIOD_CLK_ENABLE(); /* 使能GPIOD时钟 */
    __HAL_RCC_GPIOE_CLK_ENABLE(); /* 使能GPIOE时钟 */

    /* 初始化PD0,1,8,9,10,14,15 */
    gpio_init_struct.Pin =
        GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;       /* 复用推挽 */
    gpio_init_struct.Pull = GPIO_PULLUP;           /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH; /* 高速 */
    gpio_init_struct.Alternate = GPIO_AF12_FMC;    /* 复用为FMC */

    HAL_GPIO_Init(GPIOD, &gpio_init_struct); /* 初始化IO口 */

    /* 初始化PE7,8,9,10,11,12,13,14,15 */
    gpio_init_struct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
                           GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &gpio_init_struct); /* 初始化IO口 */
}

/* 功能函数 */
HAL_StatusTypeDef LCD_Clear(lcd_msg_t *msg) {
    uint32_t index = 0;
    uint32_t block_count = (lcddev.height / LCD_DMA_BUFFER_ROW) / 2;

    LCD_SetCursor(0x00, lcddev.height / 2);
    LCD_WritePrepare();

    for (uint16_t row = 0; row < LCD_DMA_BUFFER_ROW; row++) {
        for (uint16_t col = 0; col < LCD_DMA_BUFFER_COL; col++) {
            lcd_dma_buffer[row][col] = msg->color;
        }
    }

    LCD_Transmit_DMA(-LCD_DMA_BUFFER_SIZE, block_count);
}

void LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size, uint8_t mode, uint16_t color) {
    uint8_t temp, t1, t;
    uint16_t y0 = y;
    uint8_t csize = 0;
    uint8_t *pfont = 0;

    csize =
        (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); /* 得到字体一个字符对应点阵集所占的字节数 */
    chr = chr - ' '; /* 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库） */

    switch (size) {
    case 12:
        pfont = (uint8_t *)asc2_1206[chr]; /* 调用1206字体 */
        break;

    case 16:
        pfont = (uint8_t *)asc2_1608[chr]; /* 调用1608字体 */
        break;

    case 24:
        pfont = (uint8_t *)asc2_2412[chr]; /* 调用2412字体 */
        break;

    case 32:
        pfont = (uint8_t *)asc2_3216[chr]; /* 调用3216字体 */
        break;

    default:
        return;
    }

    for (t = 0; t < csize; t++) {
        temp = pfont[t]; /* 获取字符的点阵数据 */

        for (t1 = 0; t1 < 8; t1++) /* 一个字节8个点 */
        {
            if (temp & 0x80) /* 有效点,需要显示 */
            {
                LCD_DrawPoint(x, y, color); /* 画点出来,要显示这个点 */
            } else if (mode == 0)           /* 无效点并且选择非叠加方式 */
            {
                LCD_DrawPoint(
                    x, y, g_back_color); /* 画背景色,相当于这个点不显示(注意背景色由全局变量控制) */
            }

            temp <<= 1; /* 移位, 以便获取下一个位的状态 */
            y++;

            if (y >= lcddev.height)
                return; /* 超区域了 */

            if ((y - y0) == size) /* 显示完一列了? */
            {
                y = y0; /* y坐标复位 */
                x++;    /* x坐标递增 */

                if (x >= lcddev.width)
                    return; /* x坐标超区域了 */

                break;
            }
        }
    }
}

const lcd_func_t lcd_func_table[LCD_CMD_MAX] = {
    [LCD_CMD_CLEAR] = LCD_Clear,
};

void LCD_Task_Process(void *arg) {
    HAL_StatusTypeDef ret = HAL_OK;
    fmc_msg_t fmc_msg;

    lcd_mutex = osSemaphoreNew(1, 0, NULL);
    assert_param(lcd_mutex != NULL);

    LCD_SEMAPHORE_RELEASE();

    MX_LCD_Init();

    while (1) {
        osMessageQueueGet(fmc_queue, &fmc_msg, NULL, osWaitForever);
        if (fmc_msg.cmd >= LCD_CMD_MAX) {
            continue;
        }

        ret = lcd_func_table[fmc_msg.cmd](&(fmc_msg.u.lcd_msg));
        assert_param(ret == HAL_OK);
    }
}

#endif
