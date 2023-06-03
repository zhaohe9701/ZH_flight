/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 22:21:06
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-25 02:06:27
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_spi.h
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
    void readBytes(uint8_t address, uint8_t len, uint8_t *dataBuf) override;
    void writeReg(uint8_t address, uint8_t value) override;
    void writeRegs(uint8_t address, uint8_t len, uint8_t *value) override;
private:
    SPI_HandleTypeDef *_hspi;
    GPIO_TypeDef *_cs_port;
    uint16_t _cs_pin;

    void _enable();
    void _disable();
};

#endif