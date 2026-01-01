#ifndef __RTC_H__
#define __RTC_H__

#include "main.h"

#define RTC_DEFAULT_TIME                                                                           \
    {                                                                                              \
        .Hours = 0,                                                                                \
        .Minutes = 0,                                                                              \
        .Seconds = 0,                                                                              \
        .TimeFormat = 0,                                                                           \
        .SubSeconds = 0,                                                                           \
        .SecondFraction = 0,                                                                       \
        .DayLightSaving = RTC_DAYLIGHTSAVING_NONE,                                                 \
        .StoreOperation = RTC_STOREOPERATION_RESET,                                                \
    }

#define RTC_DEFAULT_DATE                                                                           \
    {                                                                                              \
        .WeekDay = 0,                                                                              \
        .Month = 1,                                                                                \
        .Date = 1,                                                                                 \
        .Year = 26,                                                                                \
    }

extern RTC_HandleTypeDef hrtc1;

/**
 * @brief Initialize the RTC
 *
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef MX_RTC_Init(void);

/**
 * @brief Set the RTC value
 *
 * @param hrtc Pointer to the RTC handle
 * @param time Pointer to the time structure
 * @param date Pointer to the date structure
 * @param overflow Overflow flag
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef MX_RTC_SetTimeDate(
    RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate, uint32_t Format);

HAL_StatusTypeDef MX_RTC_GetTimeDate(
    RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate, uint32_t Format);

HAL_StatusTypeDef MX_RTC_SetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *Alarm);

uint8_t MX_RTC_CalculateWeekDay(uint8_t year, uint8_t month, uint8_t date);

#endif
