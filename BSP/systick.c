#include "systick.h"

void HAL_SysTick_Init(void) {
    if (HAL_SYSTICK_Config(SystemCoreClock / 100000)) {
        Error_Handler();
    }
}