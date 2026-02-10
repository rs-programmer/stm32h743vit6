#include "adc.h"
#include "main.h"
#include "stm32h743xx.h"
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_adc.h"
#include "stm32h7xx_hal_cortex.h"
#include "stm32h7xx_hal_def.h"
#include "uart.h"

#define ADC1_REG_OSR 4                    // 过采样倍数
#define ADC1_REG_OVSS ADC_RIGHTBITSHIFT_2 // 过采样结果右移位数

DMA_HandleTypeDef hadc1_dma = {0};
ADC_HandleTypeDef hadc1 = {0};

bool adc1_reg_complete = false;
bool adc1_inj_complete = false;

__RAMD2_DMA uint16_t adc1_reg_value[ADC1_REG_NUM] = {0};
__RAMD2_DMA uint16_t adc1_inj_value[ADC1_INJ_NUM] = {0};

HAL_StatusTypeDef MX_ADC1_Init(void) {
    HAL_StatusTypeDef ret = HAL_OK;

    /**
     * ADC 时钟方案：
     * 1. adc_hclk: AHB12_CLK / CKMODE[1:0] = 240 MHZ / (1,2,4) 同步时钟
     * 2. adc_ker_ck: 异步时钟
     *    2.1 pll2/pll3/per_ck 作为 adc_ker_ck 的时钟源
     *    2.1 hsi/hse/csi 作为 per_ck 的时钟源 (main函数中配置)
     *    2.3 adc_ker_ck / adc_prec 作为 adc 工作时钟
     */
    /* 方案 2 */
    // RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    // PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    // PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_CLKP;
    // ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    // if (ret != HAL_OK) {
    //     Error_Handler();
    // }

    /**
     * PA5 ---> ADC_CHANNEL_19
     * PA6 ---> ADC_CHANNEL_3
     *
     * PB0 ===> ADC_CHANNEL_9
     * PB1 ===> ADC_CHANNEL_5
     */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* DMA2 配置 */
    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_LINKDMA(&hadc1, DMA_Handle, hadc1_dma);

    /* Tx DMA配置 */
    hadc1_dma.Instance = DMA2_Stream1;
    hadc1_dma.Init.Request = DMA_REQUEST_ADC1;
    hadc1_dma.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hadc1_dma.Init.PeriphInc = DMA_PINC_DISABLE;
    hadc1_dma.Init.MemInc = DMA_MINC_ENABLE;
    hadc1_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hadc1_dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hadc1_dma.Init.Mode = DMA_NORMAL;
    hadc1_dma.Init.Priority = DMA_PRIORITY_MEDIUM;
    hadc1_dma.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hadc1_dma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    hadc1_dma.Init.MemBurst = DMA_MBURST_INC4;
    hadc1_dma.Init.PeriphBurst = DMA_PBURST_SINGLE;
    ret = HAL_DMA_DeInit(&hadc1_dma);
    if (ret != HAL_OK) {
        Error_Handler();
    }
    ret = HAL_DMA_Init(&hadc1_dma);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    __HAL_RCC_ADC12_CLK_ENABLE();

    /* ADC 配置 */
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4; // 方案 1： 60 MHZ工作时钟
    hadc1.Init.Resolution = ADC_RESOLUTION_16B;           // 16 位分辨率
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;            // 扫描模式：多个通道时必须开启
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;        // 每个转换完成后，触发 EOC 中断
    hadc1.Init.LowPowerAutoWait = ENABLE;                 // 低功耗自动等待模式
    hadc1.Init.ContinuousConvMode = DISABLE;              // 连续转换
    hadc1.Init.NbrOfConversion = 2;                       // 规则组转换通道数
    hadc1.Init.DiscontinuousConvMode = DISABLE;           // 间断模式
    hadc1.Init.NbrOfDiscConversion = 0; // 规则组间断模式短序列转换通道数，注入组默认为1
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;                // 外部触发源
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; // 外部触发源触发方式
    hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;     // 数据管理模式，DMA管理
    hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;                   // 数据溢出处理方式
    hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;                 // 左移位数
    hadc1.Init.OversamplingMode = ENABLE;                  // 过采样模式 看门狗阈值匹配的是累加值
    hadc1.Init.Oversampling.Ratio = ADC1_REG_OSR;          // 采样累计次数 N
    hadc1.Init.Oversampling.RightBitShift = ADC1_REG_OVSS; // 右移位数 M : 2^M = N
    hadc1.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER; // 单触发
    hadc1.Init.Oversampling.OversamplingStopReset =
        ADC_REGOVERSAMPLING_CONTINUED_MODE; // 常规组被注入组打断后，恢复常规组采样方式

    ret = HAL_ADC_Init(&hadc1);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_19;                // 通道 19
    sConfig.Rank = ADC_REGULAR_RANK_1;               // 规则组第 1 个通道
    sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5; // 采样时间
    sConfig.SingleDiff = ADC_SINGLE_ENDED;           // 单端模式
    sConfig.OffsetNumber = ADC_OFFSET_NONE;          // 无偏移量
    sConfig.Offset = 0;                              // 偏移量为 0
    sConfig.OffsetRightShift = DISABLE;              // 偏移量右移位数
    sConfig.OffsetSignedSaturation = ENABLE;         // 偏移量有符号饱和
    ret = HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank = ADC_REGULAR_RANK_2;
    ret = HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* ADC3 */
    // sConfig.Channel = ADC_CHANNEL_VREFINT;
    // sConfig.Rank = ADC_REGULAR_RANK_3;
    // ret = HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    // if (ret != HAL_OK) {
    //     Error_Handler();
    // }

    // sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
    // sConfig.Rank = ADC_REGULAR_RANK_4;
    // ret = HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    // if (ret != HAL_OK) {
    //     Error_Handler();
    // }

    // sConfig.Channel = ADC_CHANNEL_VBAT;
    // sConfig.Rank = ADC_REGULAR_RANK_5;
    // ret = HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    // if (ret != HAL_OK) {
    //     Error_Handler();
    // }

    /* 注入组通道的配置会影响全局配置 */
    ADC_InjectionConfTypeDef sConfigInjected = {0};
    sConfigInjected.InjectedChannel = ADC_CHANNEL_9;                 // 注入组通道 2
    sConfigInjected.InjectedRank = ADC_INJECTED_RANK_1;              // 注入组第 1 个通道
    sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_2CYCLES_5; // 采样时间
    sConfigInjected.InjectedSingleDiff = ADC_SINGLE_ENDED;           // 单端模式
    sConfigInjected.InjectedOffsetNumber = ADC_OFFSET_NONE;          // 无偏移量
    sConfigInjected.InjectedOffset = 0;                              // 偏移量为 0
    sConfigInjected.InjectedOffsetRightShift = DISABLE;              // 偏移量右移位数
    sConfigInjected.InjectedOffsetSignedSaturation = ENABLE;         // 偏移量有符号饱和
    sConfigInjected.InjectedNbrOfConversion = 2;                     // 注入组转换通道数
    sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;         // 间断模式
    sConfigInjected.AutoInjectedConv = DISABLE;                      // 自动注入转换
    sConfigInjected.QueueInjectedContext = DISABLE;                  // JQM
    sConfigInjected.ExternalTrigInjecConv = ADC_SOFTWARE_START;      // 外部触发源
    sConfigInjected.ExternalTrigInjecConvEdge =
        ADC_EXTERNALTRIGINJECCONV_EDGE_NONE;        // 外部触发源触发方式
    sConfigInjected.InjecOversamplingMode = ENABLE; // 过采样模式，会覆盖规则组过采样配置
    sConfigInjected.InjecOversampling.Ratio = 4;    // 采样累计次数 N
    sConfigInjected.InjecOversampling.RightBitShift = ADC_RIGHTBITSHIFT_2; // 右移位数 M : 2^M = N
    ret = HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    sConfigInjected.InjectedChannel = ADC_CHANNEL_5;
    sConfigInjected.InjectedRank = ADC_INJECTED_RANK_2;
    ret = HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* HAL_ADC_AnalogWDGConfig */
    ADC_AnalogWDGConfTypeDef sConfigAnalogWDG = {0};
    sConfigAnalogWDG.WatchdogNumber = ADC_ANALOGWATCHDOG_1;        // 看门狗 1
    sConfigAnalogWDG.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG; // 单通道模式 仅对WDG1生效
    sConfigAnalogWDG.Channel = ADC_CHANNEL_19;                     // 通道 0
    sConfigAnalogWDG.ITMode = ENABLE;                              // 使能中断模式
    sConfigAnalogWDG.HighThreshold = 2000 * ADC1_REG_OSR;          // 高阈值 过采样倍数
    sConfigAnalogWDG.LowThreshold = 1000 * ADC1_REG_OSR;           // 低阈值 过采样倍数
    // ret = HAL_ADC_AnalogWDGConfig(&hadc1, &sConfigAnalogWDG);
    // if (ret != HAL_OK) {
    //     Error_Handler();
    // }

    ret = HAL_ADCEx_Calibration_Start(&hadc1, LL_ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(ADC_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

    return ret;
}

void ADC_IRQHandler(void) { HAL_ADC_IRQHandler(&hadc1); }
void DMA2_Stream1_IRQHandler(void) { HAL_DMA_IRQHandler(&hadc1_dma); }

void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef *hadc) {
    uart_debug("ADC1 Level Out Of Window!\r\n");
}

/**
 * @brief 规则组转换完成回调函数
 * @note 如果使用的是中断模式，需要使能EOC中断，每次完成一个规则组转换都会调用这个函数
 * @note 如果是DMA模式，需要设置DMNGT为DMA模式，所有规则组转换完成后会调用这个函数
 *
 * @param hadc
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    // static uint8_t reg_cnt = 0;
    // adc1_reg_value[reg_cnt++] = HAL_ADC_GetValue(&hadc1);
    // if (reg_cnt >= ADC1_REG_NUM) {
    //     reg_cnt = 0;
    adc1_reg_complete = true; // DMA模式
    // }
}

/**
 * @brief 注入组转换完成回调函数
 * @note 设置EOS模式，所有注入组完成转换后会调用这个函数
 *
 * @param hadc
 */
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc) {
    adc1_inj_value[0] = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);
    adc1_inj_value[1] = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_2);
    adc1_inj_complete = true;
}

void MX_ADC1_Test(void) {

    HAL_StatusTypeDef ret = HAL_OK;
    uint8_t reg_id = 0, inj_id = 0;
    bool reg_req = false, inj_req = false;

    while (1) {
        reg_id = 0;
        inj_id = 0;

        /* 规则组转换 */
        // hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV; // EOC
        // HAL_ADC_Start(&hadc1);
        // for (reg_id = 0; reg_id < ADC1_REG_NUM; reg_id++) {
        //     HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
        //     adc1_reg_value[reg_id] = HAL_ADC_GetValue(&hadc1);
        // }
        if (!reg_req) {
            reg_req = true;
            adc1_reg_complete = false;
            hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
            hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_ONESHOT; // 单次DMA数据传输
            // ret = HAL_ADC_Start_IT(&hadc1);
            ret = HAL_ADC_Start_DMA(&hadc1, adc1_reg_value, ADC1_REG_NUM);
            if (ret != HAL_OK) {
                Error_Handler();
            }
        }

        if (adc1_reg_complete) {
            reg_req = false;
            adc1_reg_complete = false;
            SCB_InvalidateDCache_by_Addr(adc1_reg_value, ADC1_REG_NUM * sizeof(uint16_t));
            for (reg_id = 0; reg_id < ADC1_REG_NUM; reg_id++) {
                uart_debug("ADC1_REG[%d] = %d\r\n", reg_id, adc1_reg_value[reg_id]);
            }
        }

        // HAL_Delay(1000);

        /* 注入组转换 */
        // hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV; // EOS
        // HAL_ADCEx_InjectedStart(&hadc1);
        // HAL_ADCEx_InjectedPollForConversion(&hadc1, HAL_MAX_DELAY);
        // adc1_inj_value[inj_id++] = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);
        // adc1_inj_value[inj_id++] = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_2);
        // HAL_ADC_Stop(&hadc1);

        if (!inj_req) {
            inj_req = true;
            adc1_inj_complete = false;
            hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
            ret = HAL_ADCEx_InjectedStart_IT(&hadc1);
            if (ret != HAL_OK) {
                Error_Handler();
            }
        }

        if (adc1_inj_complete) {
            inj_req = false;
            adc1_inj_complete = false;
            for (inj_id = 0; inj_id < ADC1_INJ_NUM; inj_id++) {
                uart_debug("ADC1_INJ[%d] = %d\r\n", inj_id, adc1_inj_value[inj_id]);
            }
        }

        HAL_Delay(1000);
    }
}
