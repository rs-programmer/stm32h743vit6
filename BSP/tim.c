#include "tim.h"
#include "main.h"
#include "stm32h7xx_hal_cortex.h"
#include "stm32h7xx_hal_tim.h"
#include "stm32h7xx_hal_tim_ex.h"
#include "uart.h"

DMA_HandleTypeDef htim6_updma = {0};
TIM_HandleTypeDef htim6 = {0};
TIM_HandleTypeDef htim15 = {0};

__RAMD2_DMA uint16_t htim15_updma_buf[TIM15_UPDMA_NUM] = {0};

/* ==============================基本定时器 TIM6/TIM7 ==================================*/

HAL_StatusTypeDef MX_TIM6_Init(void) {
    HAL_StatusTypeDef ret = HAL_OK;

    /* APB1: 240MHZ */
    __HAL_RCC_TIM6_CLK_ENABLE();

    /* DMA2 配置 */
    // __HAL_RCC_DMA2_CLK_ENABLE();
    // __HAL_LINKDMA(&htim6, hdma[TIM_DMA_ID_UPDATE], htim6_updma);
    // /* DMA ARR */
    // htim6_updma.Instance = DMA2_Stream2;
    // htim6_updma.Init.Request = DMA_REQUEST_TIM6_UP;
    // htim6_updma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    // htim6_updma.Init.PeriphInc = DMA_PINC_DISABLE;
    // htim6_updma.Init.MemInc = DMA_MINC_ENABLE;
    // htim6_updma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    // htim6_updma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    // htim6_updma.Init.Mode = DMA_CIRCULAR; // 周期性的将内存数据写入ARR中
    // htim6_updma.Init.Priority = DMA_PRIORITY_MEDIUM;
    // htim6_updma.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    // htim6_updma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    // htim6_updma.Init.MemBurst = DMA_MBURST_INC4;
    // htim6_updma.Init.PeriphBurst = DMA_PBURST_SINGLE;
    // ret = HAL_DMA_DeInit(&htim6_updma);
    // if (ret != HAL_OK) {
    //     Error_Handler();
    // }
    // ret = HAL_DMA_Init(&htim6_updma);
    // if (ret != HAL_OK) {
    //     Error_Handler();
    // }

    htim6.Instance = TIM6;
    htim6.Init.Prescaler = (24000 - 1);                           // 10 KHZ
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;                  // 仅支持向上计数
    htim6.Init.Period = (10000 - 1);                              // ARR
    htim6.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;            // 不支持分频
    htim6.Init.RepetitionCounter = 0x00;                          // 不支持重复计数
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // APRE
    ret = HAL_TIM_Base_Init(&htim6);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    ret = HAL_TIM_Base_Start(&htim6);
    // ret = HAL_TIM_Base_Start_IT(&htim6);
    // ret = HAL_TIM_Base_Start_DMA(&htim6, (uint32_t *)&htim6.Instance->ARR, 1);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    // HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 14, 0);
    // HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    /* DMA */

    return ret;
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
    RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock, uwAPB1Prescaler;
    uint32_t uwPrescalerValue;
    uint32_t pFLatency;

    /*Configure the TIM6 IRQ priority */
    if (TickPriority < (1UL << __NVIC_PRIO_BITS)) {
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, TickPriority, 0);

        /* Enable the TIM6 global Interrupt */
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
        uwTickPrio = TickPriority;
    } else {
        return HAL_ERROR;
    }

    /* Enable TIM6 clock */
    __HAL_RCC_TIM6_CLK_ENABLE();

    /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    /* Get APB1 prescaler */
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;
    /* Compute TIM6 clock */
    if (uwAPB1Prescaler == RCC_HCLK_DIV1) {
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    } else {
        uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();
    }

    /* Compute the prescaler value to have TIM6 counter clock equal to 1MHz */
    uwPrescalerValue = (uint32_t)((uwTimclock / 1000000U) - 1U);

    /* Initialize TIM6 */
    htim6.Instance = TIM6;

    /* Initialize TIMx peripheral as follow:
     * Period = [(TIM6CLK/1000) - 1]. to have a (1/1000) s time base.
     * Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
     * ClockDivision = 0
     * Counter direction = Up
     */
    htim6.Init.Period = (1000000U / 1000U) - 1U;
    htim6.Init.Prescaler = uwPrescalerValue;
    htim6.Init.ClockDivision = 0;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&htim6) == HAL_OK) {
        /* Start the TIM time Base generation in interrupt mode */
        return HAL_TIM_Base_Start_IT(&htim6);
    }

    /* Return function status */
    return HAL_ERROR;
}

void TIM6_DAC_IRQHandler(void) { HAL_TIM_IRQHandler(&htim6); }

void TIM15_IRQHandler(void) { HAL_TIM_IRQHandler(&htim15); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM6) {
        HAL_IncTick();
    }

    if (htim->Instance == TIM15) {
        uart_debug("tim15\n");
    }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM15) {
        uart_debug("tim15 oc\n");
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM15) {
        uart_debug("tim15 ic %u\n", __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1));
    }
}

void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM15) {
        uart_debug("tim15 trigger\n");
    }
}

uint32_t MX_TIM6_GetUS(void) {
    uint32_t val = __HAL_TIM_GET_COUNTER(&htim6);
    return val * 100; // 一个计数值耗时 100 us
}

/* ==============================通用定时器 TIM15(⭐)/TIM16/TIM17
 * ==================================*/
HAL_StatusTypeDef MX_TIM15_Init(void) {
    HAL_StatusTypeDef ret = HAL_OK;

    /**
     * PE4 ---> TIM15_CH1N
     * PE5 ---> TIM15_CH1
     * PE6 ---> TIM15_CH2
     */
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN; // IC / OPM输入 输入捕获时开启内部下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_TIM15;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Pull = GPIO_NOPULL; // OPM 输出
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* APB2: 240MHZ */
    __HAL_RCC_TIM15_CLK_ENABLE();

    htim15.Instance = TIM15;
    htim15.Init.Prescaler = (24000 - 1);                // 10 KHZ
    htim15.Init.CounterMode = TIM_COUNTERMODE_UP;       // 仅支持向上计数
    htim15.Init.Period = (10000 - 1);                   // ARR
    htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4; // 死区时钟 = CK_INT(APB1) / ClockDivision
    htim15.Init.RepetitionCounter = 0x00;               // 重复计数
    htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // APRE
    ret = HAL_TIM_Base_Init(&htim15);
    // if (ret != HAL_OK) {
    //     Error_Handler();
    // }

    // ret = HAL_TIM_OC_Init(&htim15);
    // ret = HAL_TIM_PWM_Init(&htim15);
    // ret = HAL_TIM_IC_Init(&htim15);
    // ret = HAL_TIM_OnePulse_Init(&htim15, TIM_OPMODE_SINGLE);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* OC 配置 */
    TIM_OC_InitTypeDef oCsConfig = {0};
    oCsConfig.OCMode = TIM_OCMODE_PWM1; // PWM1 模式
    // oCsConfig.OCMode = TIM_OCMODE_PWM2; // PWM2 模式
    // oCsConfig.OCMode = TIM_OCMODE_TIMING; // 冻结模式
    // oCsConfig.OCMode = TIM_OCMODE_TOGGLE; // 翻转模式
    oCsConfig.Pulse = 5000;
    oCsConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    oCsConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    oCsConfig.OCFastMode = TIM_OCFAST_DISABLE;
    oCsConfig.OCIdleState = TIM_OCIDLESTATE_RESET;   // TIM15 不适用
    oCsConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET; // TIM15 不适用
    // ret = HAL_TIM_OC_ConfigChannel(&htim15, &oCsConfig, TIM_CHANNEL_1);
    /* PWM通道配置时开启了CCR预加载 */
    // ret = HAL_TIM_PWM_ConfigChannel(&htim15, &oCsConfig, TIM_CHANNEL_1);
    // if (ret != HAL_OK) {
    //     Error_Handler();
    // }

    /* 开启死区插入  */
    TIM_BreakDeadTimeConfigTypeDef sDeadTimeConfig = {0};
    sDeadTimeConfig.OffStateRunMode = TIM_OSSR_ENABLE;
    sDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE; // IO状态时钟由定时器控制
    sDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sDeadTimeConfig.DeadTime = 0xFF;
    sDeadTimeConfig.BreakState = TIM_BREAK_DISABLE; // 断路功能
    sDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_LOW;
    sDeadTimeConfig.BreakFilter = 0x00;
    sDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE; // TIM15 不适用
    sDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_LOW;
    sDeadTimeConfig.Break2Filter = 0x00;
    sDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
    // ret = HAL_TIMEx_ConfigBreakDeadTime(&htim15, &sDeadTimeConfig);
    // if (ret != HAL_OK) {
    //     Error_Handler();
    // }

    /* IC 配置 */
    TIM_IC_InitTypeDef iCsConfig = {0};
    iCsConfig.ICPolarity = TIM_ICPOLARITY_RISING;
    iCsConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
    iCsConfig.ICPrescaler = TIM_ICPSC_DIV1;
    iCsConfig.ICFilter = 0x0F;
    ret = HAL_TIM_IC_ConfigChannel(&htim15, &iCsConfig, TIM_CHANNEL_1);
    // if (ret != HAL_OK) {
    //     Error_Handler();
    // }

    /* 单脉冲模式 */
    TIM_OnePulse_InitTypeDef sOnePulseConfig = {0};
    sOnePulseConfig.OCMode = TIM_OCMODE_PWM1;
    sOnePulseConfig.Pulse = 5000;
    sOnePulseConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    sOnePulseConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sOnePulseConfig.OCIdleState = TIM_OCIDLESTATE_RESET;
    sOnePulseConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    sOnePulseConfig.ICPolarity = TIM_ICPOLARITY_RISING;
    sOnePulseConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sOnePulseConfig.ICFilter = 0x0F;
    // ret = HAL_TIM_OnePulse_ConfigChannel(&htim15, &sOnePulseConfig, TIM_CHANNEL_2,
    // TIM_CHANNEL_1);
    // /* 组合复位 + 触发模式 */
    // htim15.Instance->SMCR &= ~TIM_SMCR_SMS;
    // htim15.Instance->SMCR |= TIM_SLAVEMODE_COMBINED_RESETTRIGGER;
    // if (ret != HAL_OK) {
    //     Error_Handler();
    // }

    /* 从模式: 可与IC配合使用，做到输入捕获和从模式 */
    TIM_SlaveConfigTypeDef sSlaveConfig = {0};
    // sSlaveConfig.SlaveMode = TIM_SLAVEMODE_COMBINED_RESETTRIGGER;
    sSlaveConfig.SlaveMode = TIM_SLAVEMODE_GATED;
    sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
    sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING; // 此处并不需要配置为输入模式
    sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;
    sSlaveConfig.TriggerFilter = 0x0F;
    ret = HAL_TIM_SlaveConfigSynchro(&htim15, &sSlaveConfig);
    // ret = HAL_TIM_SlaveConfigSynchro_IT(&htim15, &sSlaveConfig);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(TIM15_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(TIM15_IRQn);
    /* DMA */

    return ret;
}

HAL_StatusTypeDef MX_TIM15_Test(void) {

    HAL_StatusTypeDef ret = HAL_OK;

    /* 主模式 基础功能 */
    // ret = HAL_TIM_Base_Start(&htim15);
    // ret = HAL_TIM_Base_Start_IT(&htim15); // UIF
    /* UIF 触发DMA传输新数据到 ARR */
    // ret = HAL_TIM_Base_Start_DMA(&htim15, htim15_updma_buf, TIM6_UPDMA_NUM);

    /* OC/PWM 输出 */
    // ret |= HAL_TIM_OC_Start(&htim15, TIM_CHANNEL_1);
    // ret |= HAL_TIMEx_OCN_Start(&htim15, TIM_CHANNEL_1);
    // ret |= HAL_TIM_OC_Start_IT(&htim15, TIM_CHANNEL_1);
    // ret |= HAL_TIMEx_OCN_Start_IT(&htim15, TIM_CHANNEL_1);
    /* OC DMA 修改 CCR1 的值 修改占空比 配合重复计数值可以调整修改的频率 */
    // ret |= HAL_TIM_OC_Start_DMA(&htim15, TIM_CHANNEL_1, NULL, 0);
    // ret |= HAL_TIMEx_OCN_Start_DMA(&htim15, TIM_CHANNEL_1, NULL, 0);

    // ret |= HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
    // ret |= HAL_TIMEx_PWMN_Start(&htim15, TIM_CHANNEL_1);
    // ret |= HAL_TIM_PWM_Start_IT(&htim15, TIM_CHANNEL_1);
    // ret |= HAL_TIMEx_PWMN_Start_IT(&htim15, TIM_CHANNEL_1);
    // __HAL_TIM_ENABLE_IT(&htim15, TIM_IT_UPDATE);
    // ret |= HAL_TIM_PWM_Start_DMA(&htim15, TIM_CHANNEL_1, htim15_updma_buf, TIM15_UPDMA_NUM);
    // ret |= HAL_TIMEx_PWMN_Start_DMA(&htim15, TIM_CHANNEL_1, htim15_updma_buf, TIM15_UPDMA_NUM);

    /* IC 输入 */
    // ret = HAL_TIM_IC_Start(&htim15, TIM_CHANNEL_1);
    ret = HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
    __HAL_TIM_ENABLE_IT(&htim15, TIM_IT_UPDATE);
    // ret = HAL_TIM_IC_Start_DMA(&htim15, TIM_CHANNEL_1, NULL, 0);

    /* OPM */
    // ret = HAL_TIM_OnePulse_Start(&htim15, TIM_CHANNEL_2);

    if (ret != HAL_OK) {
        Error_Handler();
    }

    // HAL_Delay(10000);

    // HAL_TIM_OC_Stop(&htim15, TIM_CHANNEL_1);
    // HAL_TIMEx_OCN_Stop(&htim15, TIM_CHANNEL_1);
}
