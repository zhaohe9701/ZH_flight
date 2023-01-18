/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-08 01:24:46
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-17 23:15:40
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

/*状态机*/
#define STATE_MACHINE_SIGNAL    0x01
/*传感器*/
#define IMU1                Icm20689
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

/*控制*/
#define ATTITUDE_CONTROLLER_PARAM_NUM   36
#define ACTUAL_STATE_MUTEX              mutex[0]
#define EXPECT_STATE_MUTEX              mutex[1]
#define ACTUAL_STATE_SEMAPHORE          semaphore[1]
/*通信*/
#define MESSAGE_QUEUE               queue[0]
#define MESSAGE_TYPE_NUM            5
#define UART_NUM                    2
#define MESSAGE_QUEUE_MAX_LENGTH    10
/*遥控*/
#define PITCH_CHANNEL               0
#define ROLL_CHANNEL                1
#define YAW_CHANNEL                 2
#define PATTERN_CHANNEL             3
#define LOCK_CHANNEL                4

#define MANUAL_SWITCH               IBUS_SWITCH_1
#define ALTITUDE_SWITCH             IBUS_SWITCH_0
#define AUTO_SWITCH                 IBUS_SWITCH_2

#define LOCK_SWITCH                 IBUS_SWITCH_0
#define UNLOCK_SWITCH               IBUS_SWITCH_2

#define REMOTE_ATTITUDE_SCOPE   30.0f
#define REMOTE_POSITION_SCOPE   30.0f
#define REMOTE_THROTTLE_SCOPE   100.0f
#define REMOTE_PALSTANCE_SCOPE  60.0f

/*消息*/


/*循环频率*/
#define ATTITUDE_SLOVE_CYCLE_FREQ   1000
#define ATTITUDE_CONTROL_CYCLE_FREQ 500
#define POSITION_CONTROL_CYCLE_FREQ 50
#define LIGHT_CONTROL_CYCLE_FREQ    1
#define DATA_LINK_CYCLE_FREQ        50
 
/*------------------------------------------------------*/
typedef osMutexId Mutex;
typedef osSemaphoreId Semaphore;
typedef osThreadId Thread;
#define AcLock(id) osMutexWait(id, osWaitForever)
#define AcUnLock(id) osMutexRelease(id)
#define AcWaitSemaphore(id) osSemaphoreWait(id, osWaitForever)
#define AcReleaseSemaphore(id) osSemaphoreRelease(id)
#define SignalSet(signal, value) osSignalSet(signal, value);
#define SignalWait(value) osSignalWait(value, osWaitForever);

#endif