#ifndef __JSON_H__
#define __JSON_H__

#include "ac_list.h"
#include "ac_tree.h"
#include "type.h"

#define JSON_STR_MAX_LEN    512
class Json
{
private:

public:
    AC_RET CreateJsonString(AcTreeNode *root, char *buf);
};



#endif