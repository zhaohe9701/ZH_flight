/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-07 23:44:16
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-26 22:44:58
 * @FilePath: \ZH_FLIGHT\Sys\Driver\sensor_interface.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "sensor_interface.h"

Spi::Spi(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin)
{
    _hspi = hspi;
    _cs_port = cs_port;
    _cs_pin = cs_pin;
}

void Spi::ReadBytes(uint8_t address, uint8_t len, uint8_t *dataBuf)
{
    uint8_t _address = address | 0x80;
    _Enable();
    HAL_SPI_Transmit(_hspi, &_address, 1, 0x01f4);
    HAL_SPI_Receive(_hspi, dataBuf, len, 0x01f4);
    _Disable();
}

void Spi::WriteReg(uint8_t address, uint8_t value)
{
    _Enable();
    while (HAL_SPI_GetState(_hspi) == HAL_SPI_STATE_BUSY_TX_RX);
    HAL_SPI_TransmitReceive(_hspi, &address, NULL, 1, 0x01f4);
    while (HAL_SPI_GetState(_hspi) == HAL_SPI_STATE_BUSY_TX_RX);
    HAL_SPI_TransmitReceive(_hspi, &value, NULL, 1, 0x01f4);
    _Disable();
}

void Spi::_Enable()
{
    HAL_GPIO_WritePin(_cs_port, _cs_pin, GPIO_PIN_RESET);
}

void Spi::_Disable()
{
    HAL_GPIO_WritePin(_cs_port, _cs_pin, GPIO_PIN_SET);
}



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