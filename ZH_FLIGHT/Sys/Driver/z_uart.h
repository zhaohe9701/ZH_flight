/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:59:58
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-27 23:14:22
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_uart.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __Z_UART_H__
#define __Z_UART_H__

#include "main.h"
#include "communicate_interface.h"
#include "config.h"
#include "type.h"
#include "message.h"

class Uart : virtual public CommunicateInterface
{
public:
    Uart(UART_HandleTypeDef *huart, uint8_t mark);
    AC_RET Transmit(uint8_t *data, uint16_t length) override;
    bool MatchMark(uint8_t mark) override;
    bool IsMe(UART_HandleTypeDef *huart);

    Message receive_message;

    static void UartHandle(UART_HandleTypeDef *huart);
    static Uart *uart_table[UART_NUM];
    static uint8_t ind;
    
private:
    UART_HandleTypeDef *_huart = nullptr;
    uint8_t _mark = 0;
};



#endif
