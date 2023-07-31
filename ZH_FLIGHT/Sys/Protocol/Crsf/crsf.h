#ifndef __CRSF_H__
#define __CRSF_H__

#include "message_parser.h"
#include "remote_data.h"
#include "data_manager.h"
#include "type.h"

#define CRSF_HEAD           0xC8
#define CRSF_PAYLOAD        0x16
#define CRSF_CHANNEL_NUM    16
#define CRSF_BIT_LEN        11

class CrsfParser : virtual public MessageReceiveParser
{
private:
    uint16_t _channel_data[CRSF_CHANNEL_NUM] = {0};
    DataManager<RemoteData> *_manager = nullptr;
    AC_RET _decode(const uint8_t *bin, uint16_t *axis, int bit_num, int len);
public:
    MessageHead getHead() override;
    AC_RET parseMessage(uint8_t *buf, uint32_t len) override;
    void setDataManager(void *manager) override;

};

#endif