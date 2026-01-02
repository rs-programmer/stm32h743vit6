#include "main.h"
#include "delay.h"
#include "exti.h"
#include "gpio.h"
#include "rng.h"
#include "rtc.h"
#include "systick.h"
#include "uart.h"
#include "watchdog.h"
#include <stdio.h>

#define RTC_BACKUP0_LOAD 0x58585858

void SystemClock_Config(void);
static void MPU_Initialize(void);
static void MPU_Config(void);

int main(void) {
    uint32_t num = 0;
    bool flag = false;
    const char *str = "hello world\n";

    RTC_TimeTypeDef sTime = RTC_DEFAULT_TIME;
    RTC_DateTypeDef sDate = RTC_DEFAULT_DATE;
    RTC_AlarmTypeDef Alarm_a = {0};

    //    SCB_EnableICache();
    //    SCB_EnableDCache();
    HAL_Init();
    // MPU_Config();
    SystemClock_Config();

    // HAL_SysTick_Init();
    MX_GPIO_Init();
    MX_UART1_Init();
    MX_KEY_Init();
    MX_RNG_Init();
    // MX_WWDG_Init();
    MX_IWDG_Init();
    MX_RTC_Init();

    // num = HAL_RTCEx_BKUPRead(&hrtc1, RTC_BKP_DR0);
    // if (num != RTC_BACKUP0_LOAD) {
    MX_RTC_SetTimeDate(&hrtc1, &sTime, &sDate, RTC_FORMAT_BIN);
    //     HAL_RTCEx_BKUPWrite(&hrtc1, RTC_BKP_DR0, RTC_BACKUP0_LOAD);
    // }

    MX_RTC_SetAlarm(&hrtc1, NULL);

    printf("process start\n");

    while (1) {

        MX_RTC_GetTimeDate(&hrtc1, &sTime, &sDate, RTC_FORMAT_BIN);
        printf("%02d-%02d-%02d %02d:%02d:%02d %02d, week: %d\n", sDate.Year, sDate.Month,
            sDate.Date, sTime.Hours, sTime.Minutes, sTime.Seconds, sTime.SubSeconds, sDate.WeekDay);

        iwdg_feed();
        HAL_Delay(1000);
    }
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Supply configuration update enable
     */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

    /** Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
    }

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 5;
    RCC_OscInitStruct.PLL.PLLN = 160;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    RCC_OscInitStruct.PLL.PLLR = 4;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    /* 可开启 HSE HSI LSE LSI PLL1 以及 PLL1 的配置参数 */
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     * HSE: 25 MHZ
     * PLL_P: (25 / 5) * 160 / 2 = 400 MHZ
     * PLL_Q: (25 / 5) * 160 / 4 = 200 MHZ
     * PCC_R: (25 / 5) * 160 / 4 = 200 MHZ
     * 
     * SYSCLK: PLL_P = 400 MHZ
     * CPU: SYSCLK / SYSCLKDivider = 400 MHZ
     * AHB1234: SYSCLK / SYSCLKDivider / AHBCLKDivider = 200 MHZ
     * APB1234: SYSCLK / SYSCLKDivider / APB(1234)CLKDivider = 100 MHZ
     * SysTick: SYSCLK / SYSCLKDivider / (1 / 8) = 400 MHZ
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                  RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 |
                                  RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

void MPU_Config(void) {
    MPU_Region_InitTypeDef MPU_InitStruct = {0};

    /* Disables the MPU */
    HAL_MPU_Disable();

    /** Initializes and configures the Region and the memory to be protected
     */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress = 0x0;
    MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
    MPU_InitStruct.SubRegionDisable = 0x87;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    /* Enables the MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state
     */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line) {
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}
#endif
