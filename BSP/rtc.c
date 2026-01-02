#include "rtc.h"

RTC_HandleTypeDef hrtc1 = {0};

HAL_StatusTypeDef MX_RTC_Init(void) {
    HAL_StatusTypeDef ret = HAL_OK;

    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    /* 设置 RCC->BDCR(BDRST) RTC的所有寄存器和RCC->BDCR都会复位，备份区域无影响
     */
    ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    if (ret != HAL_OK) {
        Error_Handler();
    }
    __HAL_RCC_RTC_CLK_ENABLE();

    hrtc1.Instance = RTC;
    hrtc1.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc1.Init.AsynchPrediv = 127;
    hrtc1.Init.SynchPrediv = 255;
    hrtc1.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc1.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
    hrtc1.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc1.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    ret = HAL_RTC_Init(&hrtc1);
    if (ret != HAL_OK) {
        Error_Handler();
    }

    /* RTC 闹钟中断处理函数 */
    NVIC_SetPriority(RTC_Alarm_IRQn, 0);
    NVIC_EnableIRQ(RTC_Alarm_IRQn);
    /* Tamp 入侵中断 */

    /* WAKUP 唤醒中断*/
    __HAL_RCC_RTC_ENABLE();
    return ret;
}

HAL_StatusTypeDef MX_RTC_SetTimeDate(
    RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate, uint32_t Format) {
    HAL_StatusTypeDef ret = HAL_OK;

    assert_param(hrtc != NULL);
    if (sTime != NULL) {
        ret = HAL_RTC_SetTime(hrtc, sTime, Format);
        if (ret != HAL_OK) {
            Error_Handler();
        }
    }
    if (sDate != NULL) {
        sDate->WeekDay = MX_RTC_CalculateWeekDay((2000 + sDate->Year), sDate->Month, sDate->Date);
        ret = HAL_RTC_SetDate(hrtc, sDate, Format);
        if (ret != HAL_OK) {
            Error_Handler();
        }
    }
    return ret;
}

HAL_StatusTypeDef MX_RTC_GetTimeDate(
    RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate, uint32_t Format) {
    HAL_StatusTypeDef ret = HAL_OK;

    assert_param(hrtc != NULL);
    if (sTime != NULL) {
        ret = HAL_RTC_GetTime(hrtc, sTime, Format);
        if (ret != HAL_OK) {
            Error_Handler();
        }
    }
    if (sDate != NULL) {
        ret = HAL_RTC_GetDate(hrtc, sDate, Format);
        if (ret != HAL_OK) {
            Error_Handler();
        }
    }
    return ret;
}

uint8_t MX_RTC_CalculateWeekDay(uint8_t year, uint8_t month, uint8_t date) {
    uint16_t century = year / 100;
    uint16_t y = year % 100;

    if (month < 3) {
        month += 12;
        year--;
    }

    uint16_t h = (date + 13 * (month + 1) / 5 + y + y / 4 + century / 4 - 2 * century) % 7;
    return (h + 5) % 7 + 1;
}

void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc) {
    if (hrtc == &hrtc1) {
        /* 处理闹钟B事件 */
        printf("RTC Alarm B Event\n");
    }
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
    if (hrtc == &hrtc1) {
        /* 处理闹钟A事件 */
        printf("RTC Alarm A Event\n");
        MX_RTC_SetAlarm(&hrtc1, NULL);
    }
}

HAL_StatusTypeDef MX_RTC_SetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *Alarm) {
    if (Alarm == NULL) {
        RTC_AlarmTypeDef Alarm_a = {0};
        RTC_DateTypeDef sDate = {0};

        Alarm = &Alarm_a;
        MX_RTC_GetTimeDate(hrtc, &(Alarm->AlarmTime), &sDate, RTC_FORMAT_BIN);
        Alarm->AlarmTime.Minutes += 1;
        Alarm->Alarm = RTC_ALARM_A;
        Alarm->AlarmDateWeekDay = sDate.WeekDay;
        Alarm->AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
        Alarm->AlarmMask = RTC_ALARMMASK_NONE; /* 如果设置1, 则对应的时，分，秒匹配时就会触发一次 */
        Alarm->AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
    }

    return HAL_RTC_SetAlarm_IT(hrtc, Alarm, RTC_FORMAT_BIN);
}

void RTC_Alarm_IRQHandler(void) { HAL_RTC_AlarmIRQHandler(&hrtc1); }
