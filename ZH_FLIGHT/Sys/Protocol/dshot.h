/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-26 22:48:24
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-25 00:41:34
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\dshot.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __DSHOT_H__
#define __DSHOT_H__
#include "z_pwm.h"
#include "motor_protocol_interface.h"
#include "config.h"
 
#define ESC_BIT_0 8
#define ESC_BIT_1 15
#define ESC_CMD_BUF_LEN 18

#define THROTTLE_MAP(value) ((uint16_t)((value) * 20.47f))
class Dshot : virtual public MotorProtocolInterface
{
public:
    Dshot(Pwm *pwm_controller);
    void SetMotor(float value) override;
private:
    uint16_t _PreparePacket(const uint16_t value, bool request_telemetry);
    void _WriteDigital(uint16_t *motor_cmd, uint16_t value);
    Pwm *_pwm_controller = nullptr;
    uint16_t _motor_cmd[ESC_CMD_BUF_LEN] = {0};
};


#endif
