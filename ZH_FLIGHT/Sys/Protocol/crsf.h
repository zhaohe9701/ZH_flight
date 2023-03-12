#ifndef __CRSF_H__
#define __CRSF_H__

#include "message_parser.h"
#include "remote_data.h"
#include "data_manager.h"

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
    AC_RET ParseMessage(Byte *message, uint32_t length) override;
    void SetDataManager(void *manager) override;
    void Publish() override;

};

#endif