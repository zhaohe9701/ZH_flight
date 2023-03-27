#include "command_server.h"
#include "data_manager.h"
#include "message.h"
#include "print.h"
#include "type.h"
#include "sys.h"

CommandServer::CommandServer()
{
    _command_manager = new DataManager<Message>();
    _printer = new Printer(message_transmit_server->GetQueueHandle());
    _printer->SetInterfaceMark(0x01);
}

DataManager<Message> *CommandServer::GetManager()
{
    return _command_manager;
}

AC_RET CommandServer::RunCommandService()
{
    Message command = {0};
    _command_manager->Pop(&command);

    return AC_OK;
}

CommandServer::~CommandServer()
{
    delete _command_manager;
    delete _printer;
}