#ifndef __JSON_H__
#define __JSON_H__

#include "ac_list.h"
#include "ac_tree.h"
#include "type.h"

class Json
{
private:
    static AC_RET transToJsonStrCore(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len);
    static AC_RET transStructToTree(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len);
    static AC_RET transArrayToTree(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len);
    static AC_RET transValToTree(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len);
public:
    static AC_RET transTreeToJsonStr(AcTreeNode *tree, char *buf, uint32_t len);
    static AC_RET transJsonStrToTree(AcTreeNode *tree, char *buf, uint32_t len);
};



#endif