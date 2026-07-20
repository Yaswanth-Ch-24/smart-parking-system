#ifndef SERVO_H
#define SERVO_H
#include "stm32f4xx_hal.h"
void Servo_Init(TIM_HandleTypeDef *htim);
void Servo_Open(uint32_t channel);    /* 90 degrees */
void Servo_Close(uint32_t channel);   /* 0 degrees  */
#endif
