#include "json.h"
#include "ac_tree.h"
#include "type.h"
#include "Json/json.h"
#include <string.h>
#include <stdio.h>

#define BRACE_L '{'
#define BRACE_R '}'

#define SQUARE_L '['
#define SQUARE_R ']'

#define COLON ':'
#define QUOT  '"'
#define KEY "@key"
#define LEN "@len"
#define VEL "@vel"

#define VEL_STR_BUF_LEN 64
static void _TransVel(AC_DATA_TYPE type, void *data, char *str)
{
    switch (type) 
    {
        case AC_UINT8:
        case AC_UINT16:
        case AC_UINT32:
            sprintf(str, "%u", (*(uint32_t *)data));
            break;
        case AC_INT8:
        case AC_INT16:
        case AC_INT32:
            sprintf(str, "%d", (*(int32_t *)data));
            break;
        case AC_FLOAT:
        case AC_DOUBLE:
            sprintf(str, "%.4f", (*(double *)data));
            break;
        default:
            break;
    }
}
static int32_t _CreateOneNodeJsonStr(AcTreeNode *node, char *buf, int32_t ptr)
{
    char vel_str[VEL_STR_BUF_LEN] = {0};
    if (AC_NULL == node->type)
    {
        buf[ptr] = BRACE_L;
        if (nullptr != node->GetFirstChild())
        {
            _CreateOneNodeJsonStr(node->GetFirstChild(), buf, ptr + 1);
        }
    }
    if (node->len == 1)
    {
        _TransVel(node->type, node->data, vel_str);
        ptr = ptr + snprintf(buf + ptr, JSON_STR_MAX_LEN - ptr, "\"%s\":{\"%s\":\"%d\",\"%s\":\"%s\"}", node->name, LEN, node->len, VEL, vel_str);
    }
    else
    {

    }
}

AC_RET Json::CreateJsonString(AcTreeNode *root, char *buf)
{

    return AC_OK;
}