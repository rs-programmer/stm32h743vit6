#include "adc.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "delay.h"
#include "exti.h"
#include "fatfs.h"
#include "ff.h"
#include "fmc.h"
#include "gpio.h"
#include "iic.h"
#include "portmacro.h"
#include "rng.h"
#include "rtc.h"
#include "sd.h"
#include "spi.h"
#include "stm32h7xx_hal_def.h"
#include "systick.h"
#include "tim.h"
#include "uart.h"
#include "usb_device.h"
#include "watchdog.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define RTC_BACKUP0_LOAD 0x58585858

#define FATFS_MKFS_WORKSIZE (_MAX_SS)

void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Initialize(void);
static void MPU_Config(void);

osThreadId_t firstTaskHandle;
osThreadId_t testTaskHandle;

void first_task_func(void *argument) {
    FRESULT ret = FR_OK;
    DIR dir;
    uint8_t mkfs_retry = 3;
    uint8_t *work_buf = NULL;

    ret = f_mount(&SDFatFS, SDPath, 1);

    if (ret == FR_NO_FILESYSTEM) {
        uart_debug("No valid file system, start mkfs...\n");

        while (mkfs_retry--) {
            work_buf = ff_malloc(FATFS_MKFS_WORKSIZE);
            if (work_buf == NULL) {
                uart_debug("mkfs malloc work buf failed! retry: %d\n", mkfs_retry);
                continue;
            }
            ret = f_mkfs(SDPath, FM_ANY, 0, work_buf, FATFS_MKFS_WORKSIZE);
            ff_free(work_buf);
            work_buf = NULL;

            if (ret == FR_OK) {
                uart_debug("mkfs success! retry: %d\n", 3 - mkfs_retry - 1);
                f_mount(NULL, SDPath, 1);
                osDelay(pdMS_TO_TICKS(100));
                ret = f_mount(&SDFatFS, SDPath, 1);
                if (ret == FR_OK) {
                    uart_debug("f_mount after mkfs success!\n");
                    goto MOUNT_SUCCESS;
                }
            } else {
                uart_debug("mkfs failed! err: %d, retry: %d\n", ret, mkfs_retry);
            }
        }
        uart_debug("mkfs all retry failed! final err: %d\n", ret);
        goto MOUNT_FAILED;
    }

MOUNT_SUCCESS:
    FIL file;
    const char *write_str = "你好，我是nice⭐⭐⭐⭐⭐⭐测试文件\n";
    ret = f_open(&file, "0:/test.txt", FA_WRITE | FA_CREATE_ALWAYS);
    if (ret == FR_OK) {
        f_write(&file, write_str, strlen(write_str), NULL);
        f_close(&file);
        uart_debug("Write file test.txt success!\n");
    } else {
        uart_debug("f_open failed! err: %d\n", ret);
    }

MOUNT_FAILED:
    while (1) {
        uart_debug("first_task_func\n");
        LED1_TOGGLE();
        osDelay(pdMS_TO_TICKS(1000));
    }
}

void test_task_func(void *argument) {
    uint16_t cnt = 0;
    HAL_StatusTypeDef ret = HAL_OK;

    fmc_msg_t msg = {0};
    msg.cmd = LCD_CMD_CLEAR;

    MX_USB_DEVICE_Init();

    while (1) {
        uart_debug("test_task_func\n");
        // cnt += 100;
        // msg.u.lcd_msg.color = cnt;
        // MX_FMC_SendMsg(&msg);

        osDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void) {
    uint32_t num = 0;
    bool flag = false;
    const char *str = "hello world\n";

    RTC_TimeTypeDef sTime = RTC_DEFAULT_TIME;
    RTC_DateTypeDef sDate = RTC_DEFAULT_DATE;
    RTC_AlarmTypeDef Alarm_a = {0};

    SCB_EnableICache();
    SCB_EnableDCache();

    /* 缓存更新: 同步加载到RAM中的数据和函数 */
    SCB_CleanInvalidateDCache();
    SCB_InvalidateICache();

    HAL_Init();
    MPU_Config();
    SystemClock_Config();
    PeriphCommonClock_Config();

    /* 关闭PA0 PA0_C, PA1 PA1_C, PC2 PC2_C, PC3 PC3_C 引脚的模拟功能 两个引脚通过模拟开关相连 */
    HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA0, SYSCFG_SWITCH_PA0_CLOSE);
    HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA1, SYSCFG_SWITCH_PA1_CLOSE);
    HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC2, SYSCFG_SWITCH_PC2_CLOSE);
    HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC3, SYSCFG_SWITCH_PC3_CLOSE);

    mxHeapInit();
    osKernelInitialize();

    // HAL_SysTick_Init();
    MX_GPIO_Init();
    MX_UART1_Init();
    // MX_KEY_Init();
    // MX_RNG_Init();
    // // MX_WWDG_Init();
    // // MX_IWDG_Init();
    // MX_RTC_Init();
    // MX_I2C1_Init();
    // MX_SPI1_Init(SPI_MODE_MASTER);
    // MX_ADC1_Init();
    MX_TIM15_Init();
    // MX_SD_Init();

    // MX_SD_Test();

    // num = HAL_RTCEx_BKUPRead(&hrtc1, RTC_BKP_DR0);
    // if (num != RTC_BACKUP0_LOAD) {
    // MX_RTC_SetTimeDate(&hrtc1, &sTime, &sDate, RTC_FORMAT_BIN);
    //     HAL_RTCEx_BKUPWrite(&hrtc1, RTC_BKP_DR0, RTC_BACKUP0_LOAD);
    // }

    // MX_RTC_SetAlarm(&hrtc1, NULL);

    // MX_FLASH_OB_Config();
    // uart_debug("process start, revid: 0x%x, tftlcd: %d\n", HAL_GetREVID(), lcddev.id);

    MX_FATFS_Init();
    // MX_FMC_Init();

    osThreadAttr_t first_attr = {0};
    first_attr.name = "first_task";
    first_attr.priority = osPriorityNormal;
    first_attr.stack_size = 4096 * 2;
    firstTaskHandle = osThreadNew(first_task_func, NULL, &first_attr);

    osThreadAttr_t test_attr = {0};
    test_attr.name = "test_task";
    test_attr.stack_size = 4096;
    test_attr.priority = osPriorityNormal;
    // testTaskHandle = osThreadNew(test_task_func, NULL, &test_attr);

    osKernelStart();

    while (1) {
        LED1_TOGGLE();
        HAL_Delay(500);
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
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE |
                                       RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI |
                                       RCC_OSCILLATORTYPE_CSI | RCC_OSCILLATORTYPE_HSI48;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.CSIState = RCC_CSI_ON;
    RCC_OscInitStruct.CSICalibrationValue = RCC_CSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 5;
    RCC_OscInitStruct.PLL.PLLN = 192;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    RCC_OscInitStruct.PLL.PLLR = 4;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    /* 可开启 (HSE HSI LSE LSI CSI HSI48) PLL1 以及 PLL1 的配置参数 */
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     * HSE: 25 MHZ
     * PLL_P: (25 / 5) * 192 / 2 = 480 MHZ
     * PLL_Q: (25 / 5) * 192 / 4 = 240 MHZ
     * PCC_R: (25 / 5) * 192 / 4 = 240 MHZ
     *
     * SYSCLK: PLL_P = 480 MHZ
     * CPU: SYSCLK / SYSCLKDivider = 480 MHZ
     * AHB1234: SYSCLK / SYSCLKDivider / AHBCLKDivider = 240 MHZ
     * APB1234: SYSCLK / SYSCLKDivider / AHBCLKDivider/ APB(1234)CLKDivider = 120 MHZ
     * SysTick: SYSCLK / SYSCLKDivider / (1 / 8) = 480 MHZ
     *
     * D1PCLK1 --> APB3
     * D2PCLK1 --> APB1
     * D2PCLK2 --> APB2
     * D3PCLK1 --> APB4
     *
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

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void) {
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /**
     * per_ck 时钟源选择
     * CKPER: HSI 64 MHZ
     */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CKPER;
    PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_HSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        Error_Handler();
    }
}

void MPU_Config(void) {
    MPU_Region_InitTypeDef MPU_InitStruct = {0};

    /* Disables the MPU */
    HAL_MPU_Disable();

    /* DTCRAM: 0x20000000 128K */

    /* RAM: 0x24000000 512K */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress = 0x24000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* RAM_DATA: 0x24000000 64K 利用高优先级重叠特性 */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.BaseAddress = 0x24000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_64KB;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* RAM_NOT_CACHED: 0x24010000 64K */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER2;
    MPU_InitStruct.BaseAddress = 0x24010000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_64KB;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* RAM_D2: 0x30000000 288K */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER8;
    MPU_InitStruct.BaseAddress = 0x30000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* RAM_D3: 0x38000000 64K */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER12;
    MPU_InitStruct.BaseAddress = 0x38000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_64KB;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* ITCM: 0x00000000 64K */

    /* FLASH: 0x08000000 2048K */
    /* FLASH 编程时需要修改此处配置以及链接脚本文件 */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER13;
    MPU_InitStruct.BaseAddress = 0x08000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_2MB;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS; // FLASH Protected Region
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* FMC_BLOCK1_SRAM: 0x60000000 64M */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER14;
    MPU_InitStruct.BaseAddress = 0x60000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_64MB;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
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
    while (1) {
        HAL_Delay(1000);
        uart_debug("Wrong file %s on line %u\n", file, line);
    };
}
#endif
