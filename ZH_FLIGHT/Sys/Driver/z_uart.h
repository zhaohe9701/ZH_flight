/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:59:58
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-30 04:03:26
 * @FilePath: \H7B0d:\Git\ZH_flight\ZH_FLIGHT\Sys\Driver\z_uart.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __Z_UART_H__
#define __Z_UART_H__

#include "main.h"
#include "message_interface.h"
#include "config.h"

// extern "C"
// {
// void UartIRQHandler(UART_HandleTypeDef *huart);
// }
class Uart : virtual public MessageInterface
{
public:
    Uart(UART_HandleTypeDef *huart, uint8_t mark);
    void Init(uint16_t receive_length_in) override;
    void Receive() override;
    uint8_t Send(uint8_t *data, uint16_t length) override;
    bool IsMe(UART_HandleTypeDef *huart);

    static void UartHandle(UART_HandleTypeDef *huart);
    ~Uart();

    static Uart *uart_table[UART_NUM];
    uint8_t *receive_buf = nullptr;
    Message message;
    uint16_t receive_length;
private:
    UART_HandleTypeDef *_huart = nullptr;
    uint8_t _mark = 0;
};



#endif
