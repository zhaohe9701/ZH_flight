#include "command.h"
#include "Command/command.h"
#include "command.h"
#include "data_manager.h"
#include "main.h"
#include "type.h"
#include "string.h"
#include "sys.h"

#define COMMAND_HEAD 0x24

MessageHead CommandParser::GetHead()
{
    return COMMAND_HEAD;
}

AC_RET CommandParser::ParseMessage(Message &message)
{
    _manager->Push(&message);
    return AC_ERROR;
}

void CommandParser::SetDataManager(void *manager)
{
    _manager = (DataManager<Message> *)manager;
}
