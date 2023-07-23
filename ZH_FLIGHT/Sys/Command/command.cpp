#include <stdio.h>
#include "command.h"
#include "data_manager.h"
#include "type.h"
#include "string.h"
#include "sys.h"

#define COMMAND_HEAD 0x24
#define GET "get"
#define SET "set"
#define MAX_METHOD_LEN 10
MessageHead CommandParser::getHead()
{
    return COMMAND_HEAD;
}

AC_RET CommandParser::parseMessage(uint8_t *buf, uint32_t len)
{
    CommandData command;
    char method[MAX_METHOD_LEN] = {0};

    sscanf((char*)buf, "$%s %s %s", method, command.url, command.data);

    debug_printer->info("METHOD:%s URL:%s DATA:%s\n", method, command.url, command.data);
    if (0 == strcmp(GET, method))
    {
        command.method = AT_GET;
    } else if (0 == strcmp(SET, method))
    {
        command.method = AT_SET;
    } else
    {
        return AC_ERROR;
    }
    _manager->push(&command);
    return AC_OK;
}

void CommandParser::setDataManager(void *manager)
{
    _manager = (DataManager<CommandData>*)manager;
}
