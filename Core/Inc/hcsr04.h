#ifndef HCSR04_H
#define HCSR04_H
#include "stm32f4xx_hal.h"
#include <stdint.h>
void     HCSR04_Init(TIM_HandleTypeDef *htimEntry, TIM_HandleTypeDef *htimExit);
uint32_t HCSR04_MeasureEntry(void);   /* Returns distance in cm */
uint32_t HCSR04_MeasureExit(void);
#endif
