/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-22 00:00:49
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-23 03:14:53
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_uart.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "z_uart.h"
#include "os.h"
#include <string.h>


extern GlobalVar g_glob_var;

void UartHandle(UART_HandleTypeDef *huart);

extern "C"
{

/**
 * @description: 系统串口中断处理函数重写
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
        UartHandle(huart);
    }
}

}

/**
 * @description: 串口空闲中断数据处理
 * @param {UART_HandleTypeDef} *huart 串口句柄
 * @return {*}
 */
void UartHandle(UART_HandleTypeDef *huart)
{
    Message message;

    for (int i = 0; i < UART_NUM; ++i)
    {
        if (Uart::uart_table[i]->IsMe(huart))
        {
            /*从接收缓存取出数据*/
            Uart::uart_table[i]->Receive();
            /*清空接收缓存*/
            memset(Uart::uart_table[i]->receive_buf, 0, Uart::uart_table[i]->receive_length);
            /*重新打开串口中断*/
            HAL_UART_Receive_DMA(huart, Uart::uart_table[i]->receive_buf, Uart::uart_table[i]->receive_length);

            message.data = Uart::uart_table[i]->current_data;
            message.length = Uart::uart_table[i]->receive_length;
            /*消息队列发送数据引用*/
            xQueueSendFromISR(g_glob_var.queue.message_queue, &message, RETURN_IMMEDIATELY);
            
        }
    }
}

// void Upper_Computer_UART_IDLECallback(UART_HandleTypeDef *huart)
// {
//     HAL_UART_DMAStop(huart);                                            //停止本次DMA传输
//     //CDC_Transmit_FS(sbus_buff, SBUS_BUFF_SIZE);                         //测试函数：将接收到的数据打印出去
//     Upper_Computer_Receive_Parse(upper_receive_buff);
//     memset(upper_receive_buff, 0, UPPER_RECEIVE_BUFF_SIZE);                               //清零接收缓冲区
//     HAL_UART_Receive_DMA(huart, (uint8_t*)upper_receive_buff, UPPER_RECEI VE_BUFF_SIZE);   //重启开始DMA传输
// }


/**
 * @description: 
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
 * @description: 
 * @param {uint16_t} receive_length
 * @return {*}
 */
void Uart::Init(uint16_t receive_length_in)
{
    receive_length = receive_length_in;

    receive_buf = new uint8_t[receive_length];
    current_data = new uint8_t[receive_length];


}

/**
 * @description: 
 * @return {*}
 */
void Uart::Receive()
{
    memcpy(current_data, receive_buf, receive_length);
}

/**
 * @description: 
 * @param {uint8_t} *data
 * @return {*}
 */
void Uart::Send(uint8_t *data, uint16_t length)
{
    HAL_UART_Transmit_DMA(_huart, data, length);
}

/**
 * @description: 
 * @return {*}
 */
Uart::~Uart()
{
    if (receive_buf != nullptr)
    {
        delete[] receive_buf;
        delete[] current_data;
    }
}

/**
 * @description: 
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