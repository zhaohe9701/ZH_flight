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

#define MAX_URL_LEN 128
#define MAX_JSON_LEN 512
#include "data_manager.h"
#include "message_parser.h"
#include "type.h"
#include "data.h"
#include "message_manager.h"

enum CommandMethod
{
    AT_GET = 0,
    AT_SET = 1,
    AT_DOWNLOAD = 2,
};
class CommandData
{
public:
    CommandMethod method = AT_GET;
    char url[MAX_URL_LEN] = {0};
    char data[MAX_JSON_LEN] = {0};
};

class CommandParser : virtual public MessageReceiveParser
{
public:
    MessageHead getHead() override;
    AC_RET parseMessage(uint8_t *buf, uint32_t len) override;
    void setDataManager(void *manager) override;
private:
    DataManager<CommandData> *_manager = nullptr;
};

#endif