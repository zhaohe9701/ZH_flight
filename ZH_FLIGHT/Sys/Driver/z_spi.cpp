/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 22:21:16
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-11-13 22:18:10
 * @FilePath: \H7B0\Sys\Driver\z_spi.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "z_spi.h"

Spi::Spi(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin)
{
    _hspi = hspi;
    _cs_port = cs_port;
    _cs_pin = cs_pin;
}

void Spi::ReadBytes(uint8_t address, uint8_t len, uint8_t *dataBuf)
{
    _Enable();
    HAL_SPI_Transmit(_hspi, &address, 1, 0x01f4);
    HAL_SPI_Receive(_hspi, dataBuf, len, 0x01f4);
    _Disable();
}

void Spi::WriteReg(uint8_t address, uint8_t value)
{
    uint8_t ret = 0;
    _Enable();
    while (HAL_SPI_GetState(_hspi) == HAL_SPI_STATE_BUSY_TX_RX)
    {
    };
    HAL_SPI_TransmitReceive(_hspi, &address, &ret, 1, 0x01f4);

    while (HAL_SPI_GetState(_hspi) == HAL_SPI_STATE_BUSY_TX_RX)
    {
    };
    HAL_SPI_TransmitReceive(_hspi, &value, &ret, 1, 0x01f4);

    _Disable();
}

void Spi::WriteRegs(uint8_t address, uint8_t len, uint8_t *value)
{
    uint8_t ret = 0;
    _Enable();
    while (HAL_SPI_GetState(_hspi) == HAL_SPI_STATE_BUSY_TX_RX)
    {
    };
    HAL_SPI_TransmitReceive(_hspi, &address, &ret, 1, 0x01f4);

    while (HAL_SPI_GetState(_hspi) == HAL_SPI_STATE_BUSY_TX_RX)
    {
    };
    HAL_SPI_TransmitReceive(_hspi, value, &ret, len, 0x01f4);

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