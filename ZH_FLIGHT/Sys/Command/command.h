/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-02-25 01:43:38
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-12 21:10:02
 * @FilePath: \ZH_FLIGHT\Sys\Command\command.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "message_parser.h"

class CommandParser : virtual public MessageReceiveParser
{
public:
    AC_RET ParseMessage(Byte *message, uint32_t length) override;
    void SetDataManager(void *manager) override;
    void Publish() override;
private:
};

#endif