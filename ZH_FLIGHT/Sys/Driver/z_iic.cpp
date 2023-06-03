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
#include <cstring>
#include "z_iic.h"
#include "cmsis_os2.h"

extern "C"
{
    void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
    {
        Iic::active_hi2c = hi2c;
        osSemaphoreRelease(Iic::receive_semaphore);
    }
}

Iic::Iic(I2C_HandleTypeDef *hi2c, uint16_t device_address)
{
    _hi2c = hi2c;
    _device_address = device_address;
    if (Iic::receive_semaphore == nullptr)
    {
        Iic::receive_semaphore = osSemaphoreNew(1, 1, nullptr);
    }
}

void Iic::readBytes(uint8_t address, uint8_t len, uint8_t *dataBuf)
{
    while (HAL_I2C_GetState(_hi2c) != HAL_I2C_STATE_READY)
    {
    }
    osKernelLock();
    HAL_I2C_Mem_Read_DMA(_hi2c, _device_address, address, I2C_MEMADD_SIZE_8BIT, _receive_buf, len);
    osKernelUnlock();
    for(;;)
    {
        osSemaphoreAcquire(Iic::receive_semaphore, 0x10);
        if (_hi2c == active_hi2c)
        {
            memcpy(dataBuf, _receive_buf, len);
            memset(_receive_buf, 0, 32);
            break;
        }
        else
        {
            osSemaphoreRelease(Iic::receive_semaphore);
        }
    }
}

void Iic::writeReg(uint8_t address, uint8_t value)
{
    while (HAL_I2C_GetState(_hi2c) != HAL_I2C_STATE_READY)
    {
    }
    memcpy(_transmit_buf, &value, 1);
    osKernelLock();
    HAL_I2C_Mem_Write_DMA(_hi2c, _device_address, address, I2C_MEMADD_SIZE_8BIT, _transmit_buf, 1);
    osKernelUnlock();
}

void Iic::writeRegs(uint8_t address, uint8_t len, uint8_t *value)
{
    if (0 == len)
    {
        _mem_address = address;
        while (HAL_I2C_GetState(_hi2c) != HAL_I2C_STATE_READY)
        {
        }
        osKernelLock();
        HAL_I2C_Master_Transmit_DMA(_hi2c, _device_address, &_mem_address, 1);
        osKernelUnlock();
    } else
    {
        memcpy(_transmit_buf, value, len);
        while (HAL_I2C_GetState(_hi2c) != HAL_I2C_STATE_READY)
        {
        }
        osKernelLock();
        HAL_I2C_Mem_Write_DMA(_hi2c, _device_address, address, I2C_MEMADD_SIZE_8BIT,_transmit_buf, len);
        osKernelUnlock();
    }

}
I2C_HandleTypeDef *Iic::active_hi2c = nullptr;
osSemaphoreId_t Iic::receive_semaphore = nullptr;