/*
 * @brief 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-22 00:00:49
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-28 03:21:08
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_uart.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "z_uart.h"
#include "cmsis_os2.h"
#include <stdint.h>
#include <string.h>
#include "config.h"
#include "global_var.h"
#include "rtwtypes.h"
#include "type.h"

extern GlobalVar system_var;

extern "C"
{
void UartIRQHandler(UART_HandleTypeDef *huart);
}
/**
 * @brief 系统串口中断处理函数重写
 * @param {UART_HandleTypeDef} *huart
 * @return {*}
 */
void UartIRQHandler(UART_HandleTypeDef *huart)
{
    /*判断是否是空闲中断*/
    if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
    {
        /*清楚空闲中断标志（否则会一直不断进入中断）*/
        __HAL_UART_CLEAR_IDLEFLAG(huart);     
        /*调用中断处理函数*/
        Uart::uartHandle(huart);
    }
}

uint8_t Uart::ind = 0;

/**
 * @brief 串口空闲中断数据处理
 * @param {UART_HandleTypeDef} *huart 串口句柄
 * @return {*}
 */
void Uart::uartHandle(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < ind; ++i)
    {
        if (uart_table[i]->IsMe(huart))
        {
            /*消息队列发送数据*/
            osMessageQueuePut(system_var.RECEIVE_MESSAGE_QUEUE, &(uart_table[i]->receive_message), 0U, 0);
            /*清空接收缓存*/
            memset(uart_table[i]->receive_message.data, 0, uart_table[i]->receive_message.length);
            /*重新打开串口中断接收*/
            HAL_UART_Receive_DMA(huart, uart_table[i]->receive_message.data, uart_table[i]->receive_message.length);
            
        }
    }
}

/**
 * @brief 
 * @param {UART_HandleTypeDef} *huart
 * @param {uint8_t} mark
 * @return {*}
 */
Uart::Uart(UART_HandleTypeDef *huart, uint8_t mark)
{
    _mark = mark;
    _huart = huart;
    uart_table[ind] = this;
    receive_message.length = MAX_MESSAGE_LENGTH;
    ind++;
}

/**
 * @brief 
 * @param {uint8_t} *data
 * @return {*}
 */
AC_RET Uart::transmit(uint8_t *data, uint16_t length)
{
    if(HAL_OK == HAL_UART_Transmit_DMA(_huart, data, length))
    {
        return AC_OK;
    }
    else
    {
        return AC_ERROR;
    }
}

bool Uart::matchPort(uint8_t mark)
{
    if (mark == _mark)
    {
        return true;
    }
    else 
    {
        return false;
    }
}

/**
 * @brief 
 * @param {UART_HandleTypeDef} *huart
 * @return {*}
 */
bool Uart::IsMe(UART_HandleTypeDef *huart)
{
    if (huart == _huart)
    {
        return true;
    }
    else
    {
        return false;
    }
}