#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

extern SPI_HandleTypeDef  hspi2;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef  htim1;
extern TIM_HandleTypeDef  htim2;
extern TIM_HandleTypeDef  htim3;

/* ── HC-SR04 Entry ───── */
#define ENTRY_TRIG_PIN   GPIO_PIN_0
#define ENTRY_TRIG_PORT  GPIOA
#define ENTRY_ECHO_PIN   GPIO_PIN_1
#define ENTRY_ECHO_PORT  GPIOA

/* ── HC-SR04 Exit ────── */
#define EXIT_TRIG_PIN    GPIO_PIN_4
#define EXIT_TRIG_PORT   GPIOA
#define EXIT_ECHO_PIN    GPIO_PIN_0
#define EXIT_ECHO_PORT   GPIOB

/* ── Servo Gates ─────── */
/* TIM1 CH1 = Entry Gate PA8, CH2 = Exit Gate PA9 */

/* ── MAX7219 SPI2 ─────── */
#define MAX7219_CS_PIN   GPIO_PIN_12
#define MAX7219_CS_PORT  GPIOB

/* ── Slot LEDs ──────────  */
#define SLOT1_GREEN_PIN  GPIO_PIN_4
#define SLOT1_GREEN_PORT GPIOC
#define SLOT1_RED_PIN    GPIO_PIN_5
#define SLOT1_RED_PORT   GPIOC
#define SLOT2_GREEN_PIN  GPIO_PIN_13
#define SLOT2_GREEN_PORT GPIOB
#define SLOT2_RED_PIN    GPIO_PIN_14
#define SLOT2_RED_PORT   GPIOB
#define SLOT3_GREEN_PIN  GPIO_PIN_15
#define SLOT3_GREEN_PORT GPIOB
#define SLOT3_RED_PIN    GPIO_PIN_2
#define SLOT3_RED_PORT   GPIOD

/* ── Config ─────────────  */
#define TOTAL_SLOTS       3
#define VEHICLE_THRESHOLD 20   /* cm — vehicle present if dist < this */
#define SERVO_OPEN_PULSE  200  /* TIM1 ARR counts for 2ms (90 deg)    */
#define SERVO_CLOSE_PULSE 100  /* TIM1 ARR counts for 1ms (0 deg)     */

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_SPI2_Init(void);
void MX_USART1_UART_Init(void);
void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void Error_Handler(void);

#endif
