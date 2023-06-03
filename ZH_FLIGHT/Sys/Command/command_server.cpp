#include <cstring>
#include "command_server.h"
#include "data_manager.h"
#include "message.h"
#include "print.h"
#include "type.h"
#include "sys.h"
#include "json.h"

#define GET "get"
#define SET "set"

#define MAX_METHOD_LEN 4
#define MAX_URL_LEN 64
#define MAX_JSON_LEN 1024
static int32_t GetUrl(const char *cmd_buf, char *url)
{
    int i = 0;
    int j = 0;

    while (' ' == cmd_buf[i])
    {
        i++;
    }
    while (cmd_buf[i] != ' ' && cmd_buf[i] != '\r' && j < MAX_URL_LEN - 1)
    {
        url[j] = cmd_buf[i];
        i++;
        j++;
    }
    return i + 1;
}

static int32_t GetMethod(const char *cmd_buf, char *method)
{
    int i = 0;
    while (cmd_buf[i] != ' ' && i < MAX_METHOD_LEN - 1)
    {
        method[i] = cmd_buf[i];
        i++;
    }
    return i + 1;
}
CommandServer::CommandServer()
{
    _command_manager = new DataManager<Message>(3);
    _printer = new Printer(message_transmit_server->getMessageManager());
    _printer->setDecPort(0x01);
}

DataManager<Message> *CommandServer::getManager()
{
    return _command_manager;
}

AC_RET CommandServer::runCommandService()
{
    Message command;
    char method[MAX_METHOD_LEN] = {0};
    int32_t second_param_start = 0;
    do
    {
        _command_manager->pop(&command);
        if (0 == command.length)
        {
            continue;
        }
        if ('$' != command.data[0])
        {
            _printer->error("NOT COMMAND HEAD.\n");
            goto error;
        }
        if (_cmd_ptr + command.length - 1 < AT_COMMAND_MAX_LEN - 1)
        {
            memcpy(_cmd_buf + _cmd_ptr, command.data + 1, command.length - 1);
            _cmd_ptr += command.length - 1;
        }
        else
        {
            _printer->error("COMMAND BUF OVERFLOW.\n");
            goto error;
        }
    }
    while ('\n' != command.data[command.length - 1]);
    osDelay(10);
    // return AC_OK;
    _cmd_buf[_cmd_ptr - 1] = 0;
    debug_printer->info("%s\n", _cmd_buf);
    osDelay(1);
    if (_cmd_ptr < MAX_METHOD_LEN)
    {
        _printer->error("COMMAND ERROR.\n");
        goto error;
    }
    second_param_start = GetMethod(_cmd_buf, method);
    if (0 == strcmp(method, GET))
    {
        _get(_cmd_buf + second_param_start);
    }
    else if (0 == strcmp(method, SET))
    {
        _set(_cmd_buf + second_param_start);
    }
    else
    {
        _printer->error("UNKNOWN COMMAND METHOD %s.\n", method);
        goto error;
    }

    _runTempTask();

    memset(_cmd_buf, 0, AT_COMMAND_MAX_LEN);
    _cmd_ptr = 0;
    return AC_OK;
error:
    memset(_cmd_buf, 0, AT_COMMAND_MAX_LEN);
    _cmd_ptr = 0;
    return AC_ERROR;
}

CommandServer::~CommandServer()
{
    delete _command_manager;
    delete _printer;
}

void CommandServer::_get(char *command)
{
    int32_t ptr = 0;
    AcTreeNode *root = nullptr;
    AcTreeNode *node = nullptr;
    char url[MAX_URL_LEN] = {0};
    char json_buf[MAX_JSON_LEN] = {0};

    if (command[ptr] != '/')
    {
        _printer->error("URL SHOULD START WITH '\\'.\n");
        return;
    }
    ptr++;

    root = aircraft->getIndex();
    if (nullptr == root)
    {
        root = aircraft->createIndex();
    }

    GetUrl(command + ptr, url);
    debug_printer->info("url:%s\n", url);
    node = AcTree::findNode(root, url);
    if (nullptr == node)
    {
        _printer->error("CAN NOT FOUND URL.\n");
        return;
    }
    if (AC_OK != Json::transTreeToJsonStr(node, json_buf, MAX_JSON_LEN - 1))
    {
        _printer->error("TRANS TREE TO JSON STRING FAILED.\n");
    }
    _printer->info("%s\n", json_buf);
    osDelay(10);
    _printer->info("Get Finish.\n");
}

void CommandServer::_set(char *command)
{
    int32_t ptr = 0;
    AcTreeNode *root = nullptr;
    AcTreeNode *node = nullptr;
    char url[MAX_URL_LEN] = {0};

    if (command[ptr] != '/')
    {
        _printer->error("URL SHOULD START WITH '\\'.\n");
        return;
    }
    ptr++;

    root = aircraft->getIndex();
    if (nullptr == root)
    {
        root = aircraft->createIndex();
    }

    ptr = GetUrl(command + ptr, url);
    debug_printer->info("url:%s\n", url);

    node = AcTree::findNode(root, url);
    if (nullptr == node)
    {
        _printer->error("CAN NOT FOUND URL.\n");
        return;
    }
    if (AC_OK != Json::transJsonStrToTree(node, command + ptr, MAX_JSON_LEN - ptr))
    {
        _printer->error("SET DATA FAILED.\n");
    }

}

void CommandServer::_runTempTask()
{

}
