/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-08 01:24:46
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-29 22:42:21
 * @FilePath: \ZH_FLIGHT\Sys\Config\config.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <stdint.h>
#include "main.h"
#include "cmsis_os.h"
/*------------------------配置区------------------------*/

/*系统*/
#define SEMAPHORE_NUM       5
#define QUEUE_NUM           1
#define MUTEX_NUM           2
/*传感器*/
#define IMU1                Icm20602
#define IMU1_INTERFACE_OBJ  hspi1
#define IMU1_INTERFACE      Spi
#define IMU1_CS_PORT        SPI1_CS_GPIO_Port
#define IMU1_CS_PIN         SPI1_CS_Pin
#define IMU1_EXIT_PIN       SPI1_EXIT_Pin
#define IMU1_SEMAPHORE      semaphore[0]
#define MAGNETOMETER


/*电机*/
#define MOTOR_OUTPUT_MAX    100.0f
#define MOTOR_OUTPUT_MIN    0.0f
#define MOTOR_NUM           4

#define MOTOR_1_TIM                 htim8
#define MOTOR_1_CHANNEL         TIM_CHANNEL_1

#define MOTOR_2_TIM                 htim8
#define MOTOR_2_CHANNEL         TIM_CHANNEL_2

#define MOTOR_3_TIM                 htim8
#define MOTOR_3_CHANNEL         TIM_CHANNEL_3

#define MOTOR_4_TIM                 htim8
#define MOTOR_4_CHANNEL         TIM_CHANNEL_4

/*遥控*/
#define X_CHANNEL       0
#define Y_CHANNEL       2
#define Z_CHANNEL       3
#define H_CHANNEL       1
#define PATTERN_CHANNEL 4

#define REMOTE_ATTITUDE_SCOPE   30.0f
#define REMOTE_POSITION_SCOPE   30.0f
#define REMOTE_THROTTLE_SCOPE   100.0f
#define REMOTE_PALSTANCE_SCOPE  60.0f

/*控制*/
#define ATTITUDE_CONTROLLER_PARAM_NUM   36
#define ACTUAL_STATE_MUTEX              mutex[0]
#define EXPECT_STATE_MUTEX              mutex[1]
#define ACTUAL_STATE_SEMAPHORE          semaphore[1]
/*通信*/
#define UART_NUM                    2
#define MESSAGE_QUEUE_MAX_LENGTH    10

/*循环频率*/
#define ATTITUDE_SLOVE_CYCLE_FREQ   1000
#define ATTITUDE_CONTROL_CYCLE_FREQ 500
#define POSITION_CONTROL_CYCLE_FREQ 50
#define LIGHT_CONTROL_CYCLE_FREQ    50
#define DATA_LINK_CYCLE_FREQ        50
 
/*------------------------------------------------------*/
typedef osMutexId Mutex;
typedef osSemaphoreId Semaphore;
typedef uint16_t Flag;
#define AcLock(x) osMutexWait(x, osWaitForever)
#define AcUnLock(x) osMutexRelease(x)
#define AcWaitSemaphore(x) osSemaphoreWait(x, osWaitForever)
#define AcReleaseSemaphore(x) osSemaphoreRelease(x)


#endif