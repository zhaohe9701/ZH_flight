#ifndef __COMMAND_SERVER_H__
#define __COMMAND_SERVER_H__

#include "data_manager.h"
#include "data.h"
#include "print.h"
#include "type.h"
#include "command.h"

#define AT_COMMAND_MAX_LEN 1024
class CommandServer
{
private:
    void _get(CommandData &command);
    void _set(CommandData &command);
    static void _runTempTask();
    DataManager<CommandData> *_manager = nullptr;
    Printer *_printer = nullptr;
    char _cmd_buf[AT_COMMAND_MAX_LEN] = {0};
    int32_t _cmd_ptr = 0;
public:
    CommandServer();
    DataManager<CommandData> *getManager();
    AC_RET runCommandService();
    ~CommandServer();
};

#endif
