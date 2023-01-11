/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 23:06:21
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-12 00:14:29
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\ibus.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __IBUS_H__
#define __IBUS_H__

#define IBUS_LENGTH 32
#define IBUS_CHANNEL_NUM 18

#define IBUS_SWITCH_0 1000
#define IBUS_SWITCH_1 1500
#define IBUS_SWITCH_2 2000

#define IBUS_TRANS_CHANNEL_FROM_INT_TO_FLOAT(x) ((float)((x) - 1500) / 1500.0f)
#include "remote_interface.h"
#include "message_parse_interface.h"
#include "aircraft_state.h"

class IbusParser : virtual public MessageParseInterface
{
public:
    AC_RET ParseMessage(Byte *message, uint32_t length) override;
    void SetDownStream(void *carrier) override;
    void HandOut() override;
private:
    uint16_t _channel_data[IBUS_CHANNEL_NUM] = {0};
    ExpectState *_tmp_expect_state = nullptr;
    ExpectState *_expect_state = nullptr;
};
#endif