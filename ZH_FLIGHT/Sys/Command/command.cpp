#include "command.h"
#include "Command/command.h"
#include "command.h"
#include "data_manager.h"
#include "main.h"
#include "type.h"
#include "string.h"

#define COMMAND_HEAD 0x24

MessageHead CommandParser::GetHead()
{
    return COMMAND_HEAD;
}

AC_RET CommandParser::ParseMessage(Byte *message, uint32_t length)
{
    if (length < COMMAND_MAX_LEN)
    {
        memcpy(_command.data, message, length);
        return AC_OK;
    }
    return AC_ERROR;
}

void CommandParser::SetDataManager(void *manager)
{
    _manager = (DataManager<CommandData> *)manager;
}

void CommandParser::Publish()
{
    _manager->Push(&_command);
    // UsbPrintf("%s", _command.data);
}