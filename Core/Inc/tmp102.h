#ifndef TMP102_H
#define TMP102_H
#include "stm32l4xx_hal.h"
#define TMP102_I2C_ADDR (0x48U << 1)
#define TMP102_TEMP_REG 0x00U
HAL_StatusTypeDef TMP102_ReadTemp_IT(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef TMP102_GetTemperatureC(float *temperature);
#endif
