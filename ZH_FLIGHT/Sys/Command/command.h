/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-02-25 01:43:38
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-27 00:06:06
 * @FilePath: \ZH_FLIGHT\Sys\Command\command.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "data_manager.h"
#include "message_parser.h"
#include "type.h"
#include "data.h"
#include "message_manager.h"

class CommandParser : virtual public MessageReceiveParser
{
public:
    MessageHead getHead() override;
    AC_RET parseMessage(uint8_t *buf, uint32_t len) override;
    void setDataManager(void *manager) override;
private:
    MessageManager *_manager = nullptr;
};

#endif