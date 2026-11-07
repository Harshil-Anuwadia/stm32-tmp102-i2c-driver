#include "tmp102.h"

//raw 2 bytes from sensor
static uint8_t tmp102_buffer[2];

//flags
static volatile uint8_t tmp102_data_ready = 0U;
static volatile uint8_t tmp102_i2c_error = 0U;


HAL_StatusTypeDef TMP102_ReadTemp_IT(I2C_HandleTypeDef *hi2c)
{
    tmp102_data_ready = 0U;
    tmp102_i2c_error = 0U;

    return HAL_I2C_Mem_Read_IT(
        hi2c,
        TMP102_I2C_ADDR,
        TMP102_TEMP_REG,
        I2C_MEMADD_SIZE_8BIT,
        tmp102_buffer,
        2
    );
}


//when the I2C read finishes
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        tmp102_data_ready = 1U;
    }
}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        tmp102_i2c_error = 1U;
    }
}


//convert raw data to celsius
HAL_StatusTypeDef TMP102_GetTemperatureC(float *temperature)
{
    uint16_t raw_data;
    int32_t signed_data;

    if (tmp102_i2c_error != 0U)
    {
        return HAL_ERROR;
    }

    if (tmp102_data_ready == 0U)
    {
        return HAL_BUSY;
    }

    raw_data = ((uint16_t)tmp102_buffer[0] << 8) | (uint16_t)tmp102_buffer[1];

    raw_data = raw_data >> 4; // 12-bit value, see datasheet pg 8

    signed_data = (int32_t)raw_data;

    // handle negative temps (two's complement, 12-bit). msb
    if ((raw_data & 0x0800U) != 0U)
    {
        signed_data -= 4096;
    }

    *temperature = (float)signed_data * 0.0625f; // 0.0625 resolution pg 8.

    tmp102_data_ready = 0U;

    return HAL_OK;
}
