/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-26 22:48:24
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-21 00:03:01
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\dshot.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __DSHOT_H__
#define __DSHOT_H__
#include "z_pwm.h"
#include "motor_interface.h"
#include "config.h"

#define ESC_BIT_0 3
#define ESC_BIT_1 7
#define ESC_CMD_BUF_LEN 18

#define THROTTLE_MAP(value) ((uint16_t)((value) * 20.47f))
class Dshot : virtual public MotorInterface
{
public:
    void Init() override;
    void SetMotor(float *value) override;
private:
    uint16_t _PreparePacket(const uint16_t value, bool request_telemetry);
    void _WriteDigital(uint16_t *motor_cmd, uint16_t value);
    Pwm _pwmcontroller[MOTOR_NUM];
    uint16_t _motor_cmd[MOTOR_NUM][ESC_CMD_BUF_LEN] = {0};
};


#endif
