/**
 * @file  hcsr04.c
 * @brief HC-SR04 Ultrasonic driver — STM32 F446RE
 *        Entry: TRIG=PA0, ECHO=PA1 (TIM2 for timing)
 *        Exit:  TRIG=PA4, ECHO=PB0 (TIM3 for timing)
 */
#include "hcsr04.h"
#include "main.h"

static TIM_HandleTypeDef *_htimE, *_htimX;

void HCSR04_Init(TIM_HandleTypeDef *htimEntry, TIM_HandleTypeDef *htimExit)
{
    _htimE = htimEntry;
    _htimX = htimExit;
}

static uint32_t measure(GPIO_TypeDef *trigPort, uint16_t trigPin,
                         GPIO_TypeDef *echoPort, uint16_t echoPin,
                         TIM_HandleTypeDef *htim)
{
    /* Send 10us TRIG pulse */
    HAL_GPIO_WritePin(trigPort, trigPin, GPIO_PIN_RESET);
    __HAL_TIM_SET_COUNTER(htim, 0);
    while (__HAL_TIM_GET_COUNTER(htim) < 2);
    HAL_GPIO_WritePin(trigPort, trigPin, GPIO_PIN_SET);
    __HAL_TIM_SET_COUNTER(htim, 0);
    while (__HAL_TIM_GET_COUNTER(htim) < 10);
    HAL_GPIO_WritePin(trigPort, trigPin, GPIO_PIN_RESET);

    /* Wait for ECHO rising edge */
    uint32_t timeout = 100000;
    while (HAL_GPIO_ReadPin(echoPort, echoPin) == GPIO_PIN_RESET && timeout--);

    /* Measure ECHO pulse width */
    __HAL_TIM_SET_COUNTER(htim, 0);
    timeout = 100000;
    while (HAL_GPIO_ReadPin(echoPort, echoPin) == GPIO_PIN_SET && timeout--);
    uint32_t pulse_us = __HAL_TIM_GET_COUNTER(htim);

    /* Distance = pulse_us / 58  (cm) */
    return pulse_us / 58;
}

uint32_t HCSR04_MeasureEntry(void)
{
    return measure(ENTRY_TRIG_PORT, ENTRY_TRIG_PIN,
                   ENTRY_ECHO_PORT, ENTRY_ECHO_PIN, _htimE);
}

uint32_t HCSR04_MeasureExit(void)
{
    return measure(EXIT_TRIG_PORT, EXIT_TRIG_PIN,
                   EXIT_ECHO_PORT, EXIT_ECHO_PIN, _htimX);
}
