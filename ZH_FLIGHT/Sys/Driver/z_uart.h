/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:59:58
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-23 02:50:56
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_uart.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __Z_UART_H__
#define __Z_UART_H__

#include "main.h"
#include "message_interface.h"
#include "config.h"

class Uart : virtual public MessageInterface
{
public:
    Uart(UART_HandleTypeDef *huart, uint8_t mark);
    void Init(uint16_t receive_length_in) override;
    void Receive() override;
    void Send(uint8_t *data, uint16_t length) override;
    bool IsMe(UART_HandleTypeDef *huart);
    ~Uart();
    static Uart *uart_table[UART_NUM];
    uint8_t *receive_buf = nullptr;
    uint8_t *current_data = nullptr;
    uint16_t receive_length;
private:
    UART_HandleTypeDef *_huart = nullptr;
    uint8_t _mark = 0;
};



#endif
