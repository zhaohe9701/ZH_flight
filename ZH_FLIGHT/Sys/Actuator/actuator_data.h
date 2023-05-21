#ifndef __ACTUATOR_DATA_H__
#define __ACTUATOR_DATA_H__

#include "config.h"

class ActuatorData
{
public:
    float motor[MOTOR_NUM];
    float servo[SERVO_NUM];
};

#endif