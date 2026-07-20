/**
 ******************************************************************************
 * @file    main.c
 * @brief   Smart Parking Lot Management & Dynamic Guidance System
 *          Board  : STM32 F446RE (Nucleo-F446RE)
 *          Author : Chlliboina Yaswanth
 ******************************************************************************
 */

#include "main.h"
#include "hcsr04.h"
#include "servo.h"
#include "max7219.h"
#include "parking.h"
#include <stdio.h>
#include <string.h>

SPI_HandleTypeDef  hspi2;
UART_HandleTypeDef huart1;
TIM_HandleTypeDef  htim1;
TIM_HandleTypeDef  htim2;
TIM_HandleTypeDef  htim3;

static char logBuf[128];
static void UART_Print(const char *s)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)s, strlen(s), HAL_MAX_DELAY);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI2_Init();
    MX_USART1_UART_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();

    HAL_TIM_Base_Start(&htim2);
    HAL_TIM_Base_Start(&htim3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

    HCSR04_Init(&htim2, &htim3);
    Servo_Init(&htim1);
    MAX7219_Init(&hspi2);
    Parking_Init();

    UART_Print("\r\n=========================================\r\n");
    UART_Print("  Smart Parking System\r\n");
    UART_Print("  STM32 F446RE | Yaswanth Chlliboina\r\n");
    UART_Print("=========================================\r\n");

    snprintf(logBuf, sizeof(logBuf),
             "[INIT] Total slots: %d | Available: %d\r\n",
             TOTAL_SLOTS, Parking_GetAvailable());
    UART_Print(logBuf);
    MAX7219_ShowNumber(Parking_GetAvailable());

    while (1)
    {
        /* ── Entry lane check ──────────────────── */
        uint32_t entryDist = HCSR04_MeasureEntry();
        if (entryDist < VEHICLE_THRESHOLD && Parking_GetAvailable() > 0)
        {
            UART_Print("\r\n[ENTRY] Vehicle detected at entry\r\n");
            UART_Print("[GATE]  Entry gate OPENING (servo 90deg)\r\n");
            Servo_Open(TIM_CHANNEL_1);
            HAL_Delay(3000);

            uint8_t slot = Parking_AssignSlot();
            snprintf(logBuf, sizeof(logBuf),
                     "[SLOT]  Slot %d assigned >> Red LED ON\r\n", slot);
            UART_Print(logBuf);
            Parking_SetSlotLED(slot, 0); /* 0 = occupied = Red */

            snprintf(logBuf, sizeof(logBuf),
                     "[COUNT] Available: %d\r\n", Parking_GetAvailable());
            UART_Print(logBuf);
            MAX7219_ShowNumber(Parking_GetAvailable());

            if (Parking_GetAvailable() == 0)
                MAX7219_ShowText("FULL");

            Servo_Close(TIM_CHANNEL_1);
            UART_Print("[GATE]  Entry gate CLOSING\r\n");
            HAL_Delay(2000);
        }
        else if (entryDist < VEHICLE_THRESHOLD && Parking_GetAvailable() == 0)
        {
            UART_Print("[FULL]  Parking FULL — Entry gate locked\r\n");
            MAX7219_ShowText("FULL");
        }

        /* ── Exit lane check ───────────────────── */
        uint32_t exitDist = HCSR04_MeasureExit();
        if (exitDist < VEHICLE_THRESHOLD)
        {
            UART_Print("\r\n[EXIT]  Vehicle detected at exit\r\n");
            UART_Print("[GATE]  Exit gate OPENING\r\n");
            Servo_Open(TIM_CHANNEL_2);
            HAL_Delay(3000);

            uint8_t slot = Parking_ReleaseSlot();
            snprintf(logBuf, sizeof(logBuf),
                     "[SLOT]  Slot %d released >> Green LED ON\r\n", slot);
            UART_Print(logBuf);
            Parking_SetSlotLED(slot, 1); /* 1 = free = Green */

            snprintf(logBuf, sizeof(logBuf),
                     "[COUNT] Available: %d\r\n", Parking_GetAvailable());
            UART_Print(logBuf);
            MAX7219_ShowNumber(Parking_GetAvailable());

            Servo_Close(TIM_CHANNEL_2);
            UART_Print("[GATE]  Exit gate CLOSING\r\n");
            HAL_Delay(2000);
        }

        HAL_Delay(200);
    }
}

void Error_Handler(void) { while (1) {} }
