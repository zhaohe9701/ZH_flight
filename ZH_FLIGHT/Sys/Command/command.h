#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "message_parser.h"

class CommandParser : virtual public MessageReceiveParser
{
public:
    AC_RET ParseMessage(Byte *message, uint32_t length) override;
    void SetDes(void *carrier) override;
    void HandOut() override;
private:
};

#endif