#ifndef __DSHOT_H__
#define __DSHOT_H__

#include "main.h"
#include <stdbool.h>

#define ESC_BIT_0 3
#define ESC_BIT_1 7
#define ESC_CMD_BUF_LEN 18

void SetMotorValue(uint16_t *motor);
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
#endif