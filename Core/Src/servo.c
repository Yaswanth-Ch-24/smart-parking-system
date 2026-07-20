/**
 * @file  servo.c
 * @brief SG90 Servo via TIM1 PWM — STM32 F446RE
 *        50Hz signal. CCR = 100 (1ms=0deg), 200 (2ms=90deg)
 *        Assumes TIM1 ARR=20000, PSC gives 1MHz tick (1us per count)
 */
#include "servo.h"
#include "main.h"

static TIM_HandleTypeDef *_htim;

void Servo_Init(TIM_HandleTypeDef *htim) { _htim = htim; }

void Servo_Open(uint32_t channel)
{
    __HAL_TIM_SET_COMPARE(_htim, channel, SERVO_OPEN_PULSE);
    HAL_Delay(500);
}

void Servo_Close(uint32_t channel)
{
    __HAL_TIM_SET_COMPARE(_htim, channel, SERVO_CLOSE_PULSE);
    HAL_Delay(500);
}
