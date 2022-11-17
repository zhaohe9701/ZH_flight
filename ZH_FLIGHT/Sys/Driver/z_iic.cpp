/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 22:23:58
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-11-17 23:13:13
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_iic.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "z_iic.h"
#include "string.h"

extern "C"
{

    void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
    {
        Iic::active_hi2c = hi2c;
        osSemaphoreRelease(Iic::iic_semaphore);
    }

    void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
    {
        Iic::active_hi2c = hi2c;
        osSemaphoreRelease(Iic::iic_semaphore);
    }
}

Iic::Iic(I2C_HandleTypeDef *hi2c, uint16_t device_address)
{
    _hi2c = hi2c;
    _device_address = device_address;
    if (Iic::iic_semaphore == nullptr)
    {
        osSemaphoreDef(IicSemaphore);
        Iic::iic_semaphore = osSemaphoreCreate(osSemaphore(IicSemaphore), 1);
    }
}

void Iic::ReadBytes(uint8_t address, uint8_t len, uint8_t *dataBuf)
{
    HAL_I2C_Mem_Read(_hi2c, _device_address, address, I2C_MEMADD_SIZE_8BIT, dataBuf, len, 0x01f4);
    // HAL_I2C_Mem_Read_DMA(_hi2c, _device_address, address, I2C_MEMADD_SIZE_8BIT, dataBuf, len);
    // for(;;)
    // {
    //     osSemaphoreWait(Iic::iic_semaphore, osWaitForever);
    //     if (_hi2c != active_hi2c)
    //     {
    //         osSemaphoreRelease(Iic::iic_semaphore);
    //     }
    //     else
    //     {
    //         break;
    //     }
    // }
}

void Iic::WriteReg(uint8_t address, uint8_t value)
{
    HAL_I2C_Mem_Write(_hi2c, _device_address, address, I2C_MEMADD_SIZE_8BIT, &value, 1, 0x01f4);
    // HAL_I2C_Mem_Write_DMA(_hi2c, _device_address, address, I2C_MEMADD_SIZE_8BIT, &value, 1);
    // for(;;)
    // {
    //     osSemaphoreWait(Iic::iic_semaphore, osWaitForever);
    //     if (_hi2c != active_hi2c)
    //     {
    //         osSemaphoreRelease(Iic::iic_semaphore);
    //     }
    //     else
    //     {
    //         break;
    //     }
    // }
}

void Iic::WriteRegs(uint8_t address, uint8_t len, uint8_t *value)
{
    while (HAL_I2C_GetState(_hi2c) == HAL_I2C_STATE_BUSY)
    {
    }
    HAL_I2C_Master_Transmit(_hi2c, _device_address, &address, 1, 0x01f4);
    if (len == 0)
    {
        return;
    }
    while (HAL_I2C_GetState(_hi2c) == HAL_I2C_STATE_BUSY)
    {
    }
    HAL_I2C_Master_Transmit(_hi2c, _device_address, value, len, 0x01f4);
    // HAL_I2C_Master_Transmit_DMA(_hi2c, _device_address, buf, len + 1);
    // for(;;)
    // {
    //     osSemaphoreWait(Iic::iic_semaphore, osWaitForever);
    //     if (_hi2c != active_hi2c)
    //     {
    //         osSemaphoreRelease(Iic::iic_semaphore);
    //     }
    //     else
    //     {
    //         break;
    //     }
    // }
}
I2C_HandleTypeDef *Iic::active_hi2c = nullptr;
osSemaphoreId Iic::iic_semaphore = nullptr;