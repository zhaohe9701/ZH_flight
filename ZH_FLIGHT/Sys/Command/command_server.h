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
    void _Get(char* command);
    void _Set(char* command);
    static void _RunTempTask();
    DataManager<Message> *_command_manager = nullptr;
    Printer *_printer = nullptr;
    char _cmd_buf[AT_COMMAND_MAX_LEN] = {0};
    int32_t _cmd_ptr = 0;
public:
    CommandServer();
    DataManager<Message> *GetManager();
    AC_RET RunCommandService();
    ~CommandServer();
};

#endif
