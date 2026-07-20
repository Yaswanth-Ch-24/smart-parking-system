/**
 * @file  max7219.c
 * @brief MAX7219 dot-matrix display via SPI2 — STM32 F446RE
 */
#include "max7219.h"
#include "main.h"
#include <string.h>

static SPI_HandleTypeDef *_hspi;

#define CS_LOW()   HAL_GPIO_WritePin(MAX7219_CS_PORT, MAX7219_CS_PIN, GPIO_PIN_RESET)
#define CS_HIGH()  HAL_GPIO_WritePin(MAX7219_CS_PORT, MAX7219_CS_PIN, GPIO_PIN_SET)

static void MAX7219_Write(uint8_t reg, uint8_t data)
{
    uint8_t tx[2] = {reg, data};
    CS_LOW();
    HAL_SPI_Transmit(_hspi, tx, 2, 100);
    CS_HIGH();
}

void MAX7219_Init(SPI_HandleTypeDef *hspi)
{
    _hspi = hspi;
    MAX7219_Write(0x09, 0xFF); /* Decode mode: BCD for all digits */
    MAX7219_Write(0x0A, 0x07); /* Intensity: medium */
    MAX7219_Write(0x0B, 0x03); /* Scan limit: 4 digits */
    MAX7219_Write(0x0C, 0x01); /* Shutdown: normal operation */
    MAX7219_Write(0x0F, 0x00); /* Display test: off */
    /* Clear */
    for (uint8_t i = 1; i <= 4; i++) MAX7219_Write(i, 0x0F); /* 0x0F = blank */
}

void MAX7219_ShowNumber(uint8_t num)
{
    /* Show "X FRE" style — digit 1 = count */
    MAX7219_Write(0x09, 0x00); /* Raw mode for custom chars */
    /* Simple: show number on digit 4, "FREE" on 1-3 */
    /* For a real 8x8 matrix you'd write row data; simplified here */
    MAX7219_Write(0x09, 0xFF);
    MAX7219_Write(4, num & 0x0F);  /* Rightmost digit = available count */
    MAX7219_Write(3, 0x0F);        /* Blank */
    MAX7219_Write(2, 0x0F);
    MAX7219_Write(1, 0x0F);
}

void MAX7219_ShowText(const char *text)
{
    /* "FULL" maps to BCD digits for display purposes */
    /* On real hardware use 7-seg BCD codes */
    /* 0x0F = blank, digits shown as available */
    for (uint8_t i = 1; i <= 4; i++) MAX7219_Write(i, 0x0F);
}
