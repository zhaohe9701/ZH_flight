#ifndef __JSON_H__
#define __JSON_H__

#include "ac_list.h"
#include "ac_tree.h"
#include "type.h"

class Json
{
private:

public:
    static AC_RET TransTreeToJsonStr(AcTreeNode *tree, char *buf, uint32_t len);
    static AC_RET TransJsonStrToTree(AcTreeNode *tree, char *buf, uint32_t len);
};



#endif