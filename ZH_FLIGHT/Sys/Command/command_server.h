#ifndef __COMMAND_SERVER_H__
#define __COMMAND_SERVER_H__

#include "data_manager.h"
#include "message.h"
#include "print.h"
#include "type.h"

#define AT_COMMAND_MAX_LEN 1024
class CommandServer
{
private:
    void _get(char* command);
    void _set(char* command);
    static void _runTempTask();
    DataManager<Message> *_command_manager = nullptr;
    Printer *_printer = nullptr;
    char _cmd_buf[AT_COMMAND_MAX_LEN] = {0};
    int32_t _cmd_ptr = 0;
public:
    CommandServer();
    DataManager<Message> *getManager();
    AC_RET runCommandService();
    ~CommandServer();
};

#endif
