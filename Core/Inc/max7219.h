#ifndef MAX7219_H
#define MAX7219_H
#include "stm32f4xx_hal.h"
void MAX7219_Init(SPI_HandleTypeDef *hspi);
void MAX7219_ShowNumber(uint8_t num);
void MAX7219_ShowText(const char *text);
#endif
