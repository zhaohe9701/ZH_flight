#ifndef __CRSF_H__
#define __CRSF_H__

#include "message_parser.h"
#include "remote_data.h"
#include "data_manager.h"
#include "type.h"

#define CRSF_HEAD           0xC8
#define CRSF_PAYLOAD        0x16
#define CRSF_CHANNEL_NUM    16
#define CRSF_LENGTH         22

class CrsfParser : virtual public MessageReceiveParser
{
private:
    uint16_t _channel_data[CRSF_CHANNEL_NUM] = {0};
    DataManager<RemoteData> *_manager = nullptr;
public:
    MessageHead getHead() override;
    AC_RET parseMessage(Message &message) override;
    void setDataManager(void *manager) override;

};

#endif