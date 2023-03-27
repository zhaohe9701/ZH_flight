#ifndef __COMMAND_SERVER_H__
#define __COMMAND_SERVER_H__

#include "data_manager.h"
#include "message.h"
#include "print.h"
#include "type.h"
class CommandServer
{
private:
    DataManager<Message> *_command_manager = nullptr;
    Printer *_printer = nullptr;
public:
    CommandServer();
    DataManager<Message> *GetManager();
    AC_RET RunCommandService();
    ~CommandServer();
};

#endif
