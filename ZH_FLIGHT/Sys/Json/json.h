#ifndef __JSON_H__
#define __JSON_H__

#include "ac_list.h"
#include "ac_tree.h"
#include "type.h"

class Json
{
private:
    static AC_RET TransToJsonStrCore(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len);
    static AC_RET TransStructToTree(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len);
    static AC_RET TransStructArrayToTree(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len);
    static AC_RET TransValToTree(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len);
public:
    static AC_RET TransTreeToJsonStr(AcTreeNode *tree, char *buf, uint32_t len);
    static AC_RET TransJsonStrToTree(AcTreeNode *tree, char *buf, uint32_t len);
};



#endif