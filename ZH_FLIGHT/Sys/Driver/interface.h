/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-07 23:44:27
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-07-09 02:58:53
 * @FilePath: \ZH_FLIGHT\Sys\Driver\interface.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#include "main.h"

class Interface
{
public:
    virtual void ReadBytes(uint8_t address, uint8_t len, uint8_t *dataBuf) = 0;
    virtual void WriteReg(uint8_t address, uint8_t value) = 0;
};

class Spi : public Interface
{
public:
    Spi(SPI_HandleTypeDef *hspi, GPIO_TypeDef * cs_port, uint16_t cs_pin);
    void ReadBytes(uint8_t address, uint8_t len, uint8_t *dataBuf) override;
    void WriteReg(uint8_t address, uint8_t value) override;

private:
    SPI_HandleTypeDef *_hspi;
    GPIO_TypeDef * _cs_port;
    uint16_t _cs_pin;

    void _Enable();
    void _Disable();
};


class Iic : public Interface
{
public:
    Iic(I2C_HandleTypeDef *hi2c, uint16_t device_address);
    void ReadBytes(uint8_t address, uint8_t len, uint8_t *dataBuf) override;
    void WriteReg(uint8_t address, uint8_t value) override;

private:
    I2C_HandleTypeDef *_hi2c;
    uint16_t _device_address;
};

#endif