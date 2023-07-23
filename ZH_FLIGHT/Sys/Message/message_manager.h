//
// Created by zhaohe on 2023/5/25.
//

#include "type.h"
#include "data_manager.h"
#include "data.h"
#include "message.h"
#include "ac_semaphore.h"

#ifndef __MESSAGE_MANAGER_H__
#define __MESSAGE_MANAGER_H__

class MessageManager
{
public:
    explicit MessageManager(uint32_t len = 10);
    AC_RET transmit(Message &message);
    AC_RET receive(Message &message);
    ~MessageManager();
private:
    DataManager<MessageData> *_manager = nullptr;
    AcSemaphore _mutex;
};

#endif
