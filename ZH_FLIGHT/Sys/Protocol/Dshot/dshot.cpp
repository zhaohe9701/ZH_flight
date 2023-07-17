/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-26 22:48:16
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-25 00:41:41
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\dshot.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "dshot.h"


Dshot::Dshot(Pwm *pwm_controller)
{
    _pwm_controller = pwm_controller;
}

void Dshot::setMotor(float value)
{
    uint16_t int_value = 0;

    int_value = THROTTLE_MAP(value);
    _writeDigital(_motor_cmd, int_value);
    _pwm_controller->output(_motor_cmd, ESC_CMD_BUF_LEN);
}

uint16_t Dshot::_preparePacket(const uint16_t value, bool request_telemetry)
{
    // 油门大小为11位  所以这里先左移一位 添加上请求回传标志共12位
    uint16_t packet = (value << 1) | (request_telemetry ? 1 : 0);

    // 将12位数据分为3组 每组4位, 进行异或
    // compute checksum
    uint16_t csum = 0;
    uint16_t csum_data = packet;
    for (int i = 0; i < 3; i++)
    {
        csum ^= csum_data; // 异或
        csum_data >>= 4;
    }
    // 取最后四位 其他的不要
    csum &= 0xf;
    // append checksum 将CRC添加到后四位
    packet = (packet << 4) | csum;
    return packet;
}

void Dshot::_writeDigital(uint16_t *motor_cmd, uint16_t value)
{
    value = ((value > 2047) ? 2047 : value);
    value = _preparePacket(value, 0);
    motor_cmd[0]  = (value & 0x8000) ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[1]  = (value & 0x4000) ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[2]  = (value & 0x2000) ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[3]  = (value & 0x1000) ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[4]  = (value & 0x0800) ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[5]  = (value & 0x0400) ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[6]  = (value & 0x0200) ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[7]  = (value & 0x0100) ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[8]  = (value & 0x0080) ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[9]  = (value & 0x0040) ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[10] = (value & 0x0020) ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[11] = (value & 0x0010) ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[12] = (value & 0x8)    ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[13] = (value & 0x4)    ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[14] = (value & 0x2)    ? ESC_BIT_1 : ESC_BIT_0;
    motor_cmd[15] = (value & 0x1)    ? ESC_BIT_1 : ESC_BIT_0;
}