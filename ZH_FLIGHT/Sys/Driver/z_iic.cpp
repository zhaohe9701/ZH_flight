/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 22:23:58
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-26 01:00:42
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_iic.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "z_iic.h"
#include "cmsis_os2.h"
#include "string.h"

extern "C"
{
    void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
    {
        Iic::active_hi2c = hi2c;
        osSemaphoreRelease(Iic::iic_semaphore);
    }
}


static const osSemaphoreAttr_t IicSemaphore_attributes = {
  .name = "IicSemaphore"
};

Iic::Iic(I2C_HandleTypeDef *hi2c, uint16_t device_address)
{
    _hi2c = hi2c;
    _device_address = device_address;
    if (Iic::iic_semaphore == nullptr)
    {
        Iic::iic_semaphore = osSemaphoreNew(1, 1, &IicSemaphore_attributes);
    }
}

void Iic::ReadBytes(uint8_t address, uint8_t len, uint8_t *dataBuf)
{
    while (HAL_I2C_GetState(_hi2c) != HAL_I2C_STATE_READY)
    {
    } 
    HAL_I2C_Mem_Read_IT(_hi2c, _device_address, address, I2C_MEMADD_SIZE_8BIT, dataBuf, len);
    for(;;)
    {
        osSemaphoreAcquire(Iic::iic_semaphore, 0x10);
        if (_hi2c == active_hi2c)
        {
            // osDelay(1);
            break;
        }
        else
        {
            osSemaphoreRelease(Iic::iic_semaphore);
        }
    }
}

void Iic::WriteReg(uint8_t address, uint8_t value)
{
    while (HAL_I2C_GetState(_hi2c) != HAL_I2C_STATE_READY)
    {
    }
    HAL_I2C_Mem_Write_IT(_hi2c, _device_address, address, I2C_MEMADD_SIZE_8BIT, &value, 1);
}

void Iic::WriteRegs(uint8_t address, uint8_t len, uint8_t *value)
{
    _mem_address = address;
    while (HAL_I2C_GetState(_hi2c) != HAL_I2C_STATE_READY)
    {
    }
    HAL_I2C_Master_Transmit_IT(_hi2c, _device_address, &_mem_address, 1);
    if (len == 0)
    {
        return;
    }
    while (HAL_I2C_GetState(_hi2c) != HAL_I2C_STATE_READY)
    {
    }
    HAL_I2C_Master_Transmit_IT(_hi2c, _device_address, value, len);
    while (HAL_I2C_GetState(_hi2c) != HAL_I2C_STATE_READY)
    {
    }
}
I2C_HandleTypeDef *Iic::active_hi2c = nullptr;
osSemaphoreId_t Iic::iic_semaphore = nullptr;