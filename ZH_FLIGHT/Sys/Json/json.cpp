#include <cstring>
#include "json.h"
#include "ac_tree.h"
#include "sys.h"


#define AddElementWithCheck(element)                \
{                                                   \
if (AC_OK != AddElement(buf, (element), ptr, len))  \
    goto error;                                     \
}

static AC_RET GotoSymbol(const char *buf, uint32_t &ptr, char symbol)
{
    while (buf[ptr] != 0)
    {
        if (buf[ptr] == symbol)
        {
            return AC_OK;
        }
        ptr++;
    }
    return AC_ERROR;
}

static char GetAndGoToNextSymbol(const char *buf, uint32_t &ptr)
{
    while (buf[ptr] == ' ')
    {
        ptr++;
    }
    return buf[ptr];
}
static AC_RET GetElement(const char *buf, char *element, uint32_t &ptr, uint32_t len)
{
    uint32_t i = 0;
    if (nullptr == buf || nullptr == element)
    {
        return AC_ERROR;
    }

    while (ptr < len && buf[ptr] == ' ')
    {
        ptr++;
    }
    if (ptr == len || buf[ptr] != '\"')
    {
        return AC_ERROR;
    }

    while (ptr < len && buf[ptr] != '\"')
    {
        element[i] = buf[ptr];
        i++;
        ptr++;
    }
    if (ptr == len)
    {
        return AC_ERROR;
    }
    ptr++;
    return AC_OK;
}

static AC_RET AddElement(char *buf, const char *element, uint32_t &ptr, uint32_t len)
{
    uint32_t str_len = 0;

    str_len = strlen(element);
    if (ptr + str_len > len - 1)
    {
        return AC_ERROR;
    }
    memcpy(buf + ptr, element, str_len);
    ptr += str_len;
    return AC_OK;
}

static AC_RET AddValToJsonStr(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len, uint16_t index)
{
    char type_buf[TYPE_BUF_LEN] = {0};
    char data_buf[DATA_BUF_LEN] = {0};
    AddElementWithCheck("{");
    AddElementWithCheck("\"@t\":");
    if (AC_OK != Type::MatchType(type_buf, node->type))
    {
        goto error;
    }
    AddElementWithCheck("\"");
    AddElementWithCheck(type_buf);
    AddElementWithCheck("\"");

    AddElementWithCheck(",");
    AddElementWithCheck("\"@v\":");
    if (AC_OK != Type::TransDataToStr(data_buf, node->data, node->type, index))
    {
        goto error;
    }
    AddElementWithCheck("\"");
    AddElementWithCheck(data_buf);
    AddElementWithCheck("\"");
    AddElementWithCheck("}");
    return AC_OK;
    error:
    return AC_ERROR;
}

static AC_RET TransToJsonStrCore(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len)
{
    if (nullptr == node)
    {
        goto error;
    }
    if (0 != node->name[0])
    {
        AddElementWithCheck("\"");
        AddElementWithCheck(node->name);
        AddElementWithCheck("\"");
        AddElementWithCheck(":");
    }

    if (AC_STRUCT == node->type)
    {
        AddElementWithCheck("{");
        AcTreeNode *child = node->GetFirstChild();

        while (nullptr != child)
        {
            if (child != node->GetFirstChild())
            {
                AddElementWithCheck(",");
            }
            if (AC_OK != TransToJsonStrCore(child, buf, ptr, len))
            {
                goto error;
            }
            child = child->GetNeighbor();
        }
        AddElementWithCheck("}");
    } else if (AC_STRUCT_ARRAY == node->type)
    {
        AddElementWithCheck("[");

        AcTreeNode *child = node->GetFirstChild();

        while (nullptr != child)
        {
            if (child != node->GetFirstChild())
            {
                AddElementWithCheck(",");
            }
            if (AC_OK != TransToJsonStrCore(child, buf, ptr, len))
            {
                goto error;
            }
            child = child->GetNeighbor();
        }

        AddElementWithCheck("]");
        debug_printer->Info("result:%s\n", buf);
        osDelay(10);
    } else if (AC_BASIC_ARRAY == node->type)
    {
        AddElementWithCheck("[");
        for (uint16_t i = 0; i < node->len; ++i)
        {
            if (AC_OK != AddValToJsonStr(node, buf, ptr, len, i))
            {
                goto error;
            }
            if (i < node->len - 1)
            {
                AddElementWithCheck(",");
            }
        }
        AddElementWithCheck("]");
        debug_printer->Info("result:%s\n", buf);
        osDelay(10);
    }
    else
    {
        if (AC_OK != AddValToJsonStr(node, buf, ptr, len, 0))
        {
            goto error;
        }
        debug_printer->Info("result:%s\n", buf);
        osDelay(10);
    }
    return AC_OK;
    error:
    return AC_ERROR;
}

static AC_RET TransStructToTree(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len)
{
    char element_buf[DATA_BUF_LEN] = {0};
    char next = 0;
    ptr++;
    while ('\"' == GetAndGoToNextSymbol(buf, ptr))
    {
        if (AC_OK != GetElement(buf, element_buf, ptr, len))
        {
            goto error;
        }
        if (':' != GetAndGoToNextSymbol(buf, ptr))
        {
            goto error;
        }
        AcTreeNode *child = AcTree::FindChildByName(node, element_buf);
        if (nullptr == child)
        {
            goto error;
        }
        if (AC_STRUCT == child->type)
        {
            if ('{' != GetAndGoToNextSymbol(buf, ptr))
            {
                goto error;
            }
            if (AC_OK != TransStructToTree(child, buf, ptr, len))
            {
                goto error;
            }
        } else if (AC_STRUCT_ARRAY == child->type)
        {
            if ('[' != GetAndGoToNextSymbol(buf, ptr))
            {
                goto error;
            }
//            if (AC_OK != TransStructArrayToTree(child, buf, ptr, len))
//            {
//                goto error;
//            }
        } else if (AC_BASIC_ARRAY == child->type)
        {
            if ('[' != GetAndGoToNextSymbol(buf, ptr))
            {
                goto error;
            }
//            if (AC_OK != TransBasicArrayToTree(child, buf, ptr, len))
//            {
//                goto error;
//            }
        } else
        {
            if ('{' != GetAndGoToNextSymbol(buf, ptr))
            {
                goto error;
            }
//            if (AC_OK != TransValToTree(child, buf, ptr, len))
//            {
//                goto error;
//            }
        }
        next = GetAndGoToNextSymbol(buf, ptr);
        if (',' == next)
        {
            ptr++;
        } else if ('}' == next)
        {
            ptr++;
            break;
        } else
        {
            goto error;
        }
    }

    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Json::TransTreeToJsonStr(AcTreeNode *tree, char *buf, uint32_t len)
{
    uint32_t ptr = 0;
    AddElementWithCheck("{");
    if (AC_OK != TransToJsonStrCore(tree, buf, ptr, len))
    {
        goto error;
    }
    AddElementWithCheck("}");
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Json::TransJsonStrToTree(AcTreeNode *tree, char *buf, uint32_t len)
{
    return AC_ERROR;
}
