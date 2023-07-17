#include <cstring>
#include "command_server.h"
#include "data_manager.h"
#include "data.h"
#include "print.h"
#include "type.h"
#include "sys.h"
#include "json.h"

#define MAX_RESULT_LEN 1024

CommandServer::CommandServer()
{
    _manager = new DataManager<CommandData>();
    _printer = new Printer(message_transmit_server->getMessageManager());
    _printer->setDecPort(0x01);
}

DataManager<CommandData> *CommandServer::getManager()
{
    return _manager;
}

AC_RET CommandServer::runCommandService()
{
    CommandData command;

    _manager->pop(&command);

    if (AT_GET == command.method)
    {
        _get(command);
    } else if (AT_SET == command.method)
    {
        _set(command);
    } else
    {
        goto error;
    }

    _runTempTask();

    memset(_cmd_buf, 0, AT_COMMAND_MAX_LEN);
    _cmd_ptr = 0;
    return AC_OK;
error:
    memset(_cmd_buf, 0, AT_COMMAND_MAX_LEN);
    return AC_ERROR;
}

CommandServer::~CommandServer()
{
    delete _manager;
    delete _printer;
}

void CommandServer::_get(CommandData &command)
{
    AcTreeNode *root = nullptr;
    AcTreeNode *node = nullptr;
    char json_buf[MAX_RESULT_LEN] = {0};

    root = aircraft->getIndex();
    if (nullptr == root)
    {
        root = aircraft->createIndex();
    }
    node = AcTree::findNode(root, command.url + 1);
    if (nullptr == node)
    {
        _printer->error("CAN NOT FOUND URL.\n");
        return;
    }
    if (AC_OK != Json::transTreeToJsonStr(node, json_buf, MAX_RESULT_LEN - 1))
    {
        _printer->error("TRANS TREE TO JSON STRING FAILED.\n");
    }
    _printer->info("%s\n", json_buf);
}

void CommandServer::_set(CommandData &command)
{
    int32_t ptr = 0;
    AcTreeNode *root = nullptr;
    AcTreeNode *node = nullptr;

    root = aircraft->getIndex();
    if (nullptr == root)
    {
        root = aircraft->createIndex();
    }

    node = AcTree::findNode(root, command.url);
    if (nullptr == node)
    {
        _printer->error("CAN NOT FOUND URL.\n");
        return;
    }
    if (AC_OK != Json::transJsonStrToTree(node, command.data, MAX_JSON_LEN - ptr))
    {
        _printer->error("SET DATA FAILED.\n");
    }

}

void CommandServer::_runTempTask()
{

}
