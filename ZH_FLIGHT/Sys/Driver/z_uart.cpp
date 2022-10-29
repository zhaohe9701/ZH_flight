/*
 * @brief 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-22 00:00:49
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-30 04:07:10
 * @FilePath: \H7B0d:\Git\ZH_flight\ZH_FLIGHT\Sys\Driver\z_uart.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "z_uart.h"
#include "os.h"
#include <string.h>

extern GlobalVar g_glob_var;


extern "C"
{

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
        Uart::UartHandle(huart);
    }
}

}

/**
 * @brief 串口空闲中断数据处理
 * @param {UART_HandleTypeDef} *huart 串口句柄
 * @return {*}
 */
void Uart::UartHandle(UART_HandleTypeDef *huart)
{

    for (int i = 0; i < UART_NUM; ++i)
    {
        if (Uart::uart_table[i]->IsMe(huart))
        {
            /*从接收缓存取出数据*/
            Uart::uart_table[i]->Receive();
            /*清空接收缓存*/
            memset(Uart::uart_table[i]->receive_buf, 0, Uart::uart_table[i]->receive_length);
            /*重新打开串口中断接收*/
            HAL_UART_Receive_DMA(huart, Uart::uart_table[i]->receive_buf, Uart::uart_table[i]->receive_length);
            /*消息队列发送数据引用*/
            xQueueSendFromISR(g_glob_var.queue.message_queue, &(Uart::uart_table[i]->message), RETURN_IMMEDIATELY);
            
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
    _huart = huart;
    _mark = mark;
    uart_table[mark] = this;
}

/**
 * @brief 串口初始化
 * @param {uint16_t} receive_length
 * @return {*}
 */
void Uart::Init(uint16_t receive_length_in)
{
    receive_length = receive_length_in;
    message.length = receive_length_in;
    receive_buf = new uint8_t[receive_length];
    message.data = new uint8_t[receive_length];
    HAL_UART_Receive_DMA(_huart, receive_buf, receive_length);
}

/**
 * @brief 
 * @return {*}
 */
void Uart::Receive()
{
    memcpy(message.data, receive_buf, receive_length);
}

/**
 * @brief 
 * @param {uint8_t} *data
 * @return {*}
 */
uint8_t Uart::Send(uint8_t *data, uint16_t length)
{
    return (uint8_t)HAL_UART_Transmit_DMA(_huart, data, length);
}

/**
 * @brief 
 * @return {*}
 */
Uart::~Uart()
{
    if (nullptr != receive_buf)
    {
        delete[] receive_buf;
    }
    if (nullptr != message.data)
    {
        delete[] message.data;
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