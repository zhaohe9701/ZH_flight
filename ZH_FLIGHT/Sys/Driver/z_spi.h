/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 22:21:06
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-11-13 22:18:29
 * @FilePath: \H7B0\Sys\Driver\z_spi.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __Z_SPI_H__
#define __Z_SPI_H__

#include "sensor_interface.h"
#include "spi.h"

class Spi : public SensorInterface
{
public:
    Spi(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin);
    void ReadBytes(uint8_t address, uint8_t len, uint8_t *dataBuf) override;
    void WriteReg(uint8_t address, uint8_t value) override;
    void WriteRegs(uint8_t address, uint8_t len, uint8_t *value) override;

private:
    SPI_HandleTypeDef *_hspi;
    GPIO_TypeDef *_cs_port;
    uint16_t _cs_pin;

    void _Enable();
    void _Disable();
};

#endif