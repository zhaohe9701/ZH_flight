/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 22:23:58
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-27 22:25:45
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_iic.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "z_iic.h"

Iic::Iic(I2C_HandleTypeDef *hi2c, uint16_t device_address)
{
    _hi2c = hi2c;
    _device_address = _device_address;
}

void Iic::ReadBytes(uint8_t address, uint8_t len, uint8_t *dataBuf)
{
    HAL_I2C_Mem_Read(_hi2c, _device_address, address, I2C_MEMADD_SIZE_8BIT, dataBuf, len, 0x01f4);
}

void Iic::WriteReg(uint8_t address, uint8_t value)
{
    HAL_I2C_Mem_Write(_hi2c, _device_address, address, I2C_MEMADD_SIZE_8BIT, &value, 1, 0x01f4);
}