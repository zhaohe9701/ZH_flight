/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-26 22:48:24
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-27 00:02:57
 * @FilePath: \ZH_FLIGHT\Sys\Motor\dshot.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __DSHOT_H__
#define __DSHOT_H__
#include "pwm.h"
#include "motor.h"
#include "config.h"

#define ESC_BIT_0 3
#define ESC_BIT_1 7
#define ESC_CMD_BUF_LEN 18

class Dshot : virtual public Motor
{
public:
    void Init() override;
    void EnableMotor() override;
    void SetMotor(uint16_t *value) override;
private:
    uint16_t _PreparePacket(const uint16_t value, bool request_telemetry);
    void _WriteDigital(uint16_t *motor_cmd, uint16_t value);
    Pwm _pwmcontroller[MOTOR_NUM];
    uint16_t _motor_cmd[MOTOR_NUM][ESC_CMD_BUF_LEN] = {0};
};


#endif
