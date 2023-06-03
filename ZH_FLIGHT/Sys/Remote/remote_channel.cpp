#include "remote_channel.h"
#include "ac_tree.h"
#include "config.h"
#include "type.h"
#include <string.h>


RemoteChannelMapper::RemoteChannelMapper()
{
    _channel_map[X_CHANNEL] = X_CHANNEL;
    _channel_map[Y_CHANNEL] = Y_CHANNEL;
    _channel_map[Z_CHANNEL] = Z_CHANNEL;
    _channel_map[PATTERN_CHANNEL] = PATTERN_CHANNEL;
    _channel_map[LOCK_CHANNEL] = LOCK_CHANNEL;
}

AcTreeNode* RemoteChannelMapper::paramRegister()
{
    AcTreeNode *root = new AcTreeNode();
    strcpy(root->name, "RemoteChannel");
    root->type = AC_UINT8;
    root->len = TOTAL_CHANNEL_NUM;
    root->data = (void *)_channel_map;
    return root;
}

RemoteChannelMapper::~RemoteChannelMapper()
{

}