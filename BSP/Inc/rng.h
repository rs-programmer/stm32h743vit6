#ifndef __RNG_H__
#define __RNG_H__

#include "main.h"
#include <stdbool.h>

#define rand(num) HAL_RNG_GenerateRandomNumber(&hrng1, &num)
#define rand_it() HAL_RNG_GenerateRandomNumber_IT(&hrng1)
#define rand_last() HAL_RNG_ReadLastRandomNumber(&hrng1)

extern RNG_HandleTypeDef hrng1;
extern uint32_t random;

HAL_StatusTypeDef MX_RNG_Init(void);

#endif
