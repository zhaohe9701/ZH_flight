/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 22:23:50
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-25 02:06:12
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_iic.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __Z_IIC_H__
#define __Z_IIC_H__

#include "sensor_interface.h"
#include "cmsis_os2.h"
#include "i2c.h"
// #include "i2c.h"

class Iic : public SensorInterface
{
public:
    Iic(I2C_HandleTypeDef *hi2c, uint16_t device_address);
    void ReadBytes(uint8_t address, uint8_t len, uint8_t *dataBuf) override;
    void WriteReg(uint8_t address, uint8_t value) override;
    void WriteRegs(uint8_t address, uint8_t len, uint8_t *value) override;
    static I2C_HandleTypeDef *active_hi2c;
    static osSemaphoreId_t receive_semaphore;
private:
    I2C_HandleTypeDef *_hi2c;
    uint8_t _mem_address = 0;
    uint16_t _device_address;
    uint8_t _receive_buf[32] = {0};
    uint8_t _transmit_buf[32] = {0};
    
};



#endif