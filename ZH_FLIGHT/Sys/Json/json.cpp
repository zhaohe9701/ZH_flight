#include <cstring>
#include "json.h"
#include "ac_tree.h"
#include "sys.h"


#define addElementWithCheck(element)                \
{                                                   \
if (AC_OK != AddElement(buf, (element), ptr, len))  \
    goto error;                                     \
}

static AC_RET gotoSymbol(const char *buf, uint32_t &ptr, char symbol)
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

static char getAndGoToNextSymbol(const char *buf, uint32_t &ptr)
{
    while (buf[ptr] == ' ')
    {
        ptr++;
    }
    return buf[ptr];
}

static AC_RET getElement(const char *buf, char *element, uint32_t &ptr, uint32_t len)
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
    ptr++;
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

static AC_RET getOneVal(const char *buf, char *element, uint32_t &ptr, uint32_t len)
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

    while (ptr < len && buf[ptr] != ' ' && buf[ptr] != '\"')
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

static AC_RET addValToJsonStr(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len)
{
    char type_buf[TYPE_BUF_LEN] = {0};
    char data_buf[DATA_BUF_LEN] = {0};
    addElementWithCheck("{")
    addElementWithCheck("\"@t\":")
    if (AC_OK != Type::transTypeToStr(type_buf, node->type))
    {
        debug_printer->error("trans type to string failed.\n");
        goto error;
    }
    addElementWithCheck("\"")
    addElementWithCheck(type_buf)
    addElementWithCheck("\"")

    addElementWithCheck(",")
    addElementWithCheck("\"@v\":")
    addElementWithCheck("\"")
    for (uint16_t i = 0; i < node->len; ++i)
    {
        if (AC_OK != Type::transDataToStr(data_buf, node->data, node->type, i))
        {
            goto error;
        }
        addElementWithCheck(data_buf)
        if (i < node->len - 1)
        {
            addElementWithCheck(" ")
        }
    }
    addElementWithCheck("\"")
    addElementWithCheck("}")
    return AC_OK;
    error:
    return AC_ERROR;
}

static AC_RET checkTypeWithTreeNode(AcTreeNode *node, char *type_buf)
{
    AC_DATA_TYPE type = AC_NULL;
    if (AC_OK != Type::transStrToType(type_buf, type))
    {
        debug_printer->error("trans string to type failed.\n");
        goto error;
    }
    if (type != node->type)
    {
        goto error;
    }
    return AC_OK;
    error:
    return AC_ERROR;
}

static AC_RET addValueToTreeNode(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len)
{
    char element_buf[DATA_BUF_LEN] = {0};

    ptr++;
    for (uint16_t i = 0; i < node->len; ++i)
    {
        if (i >= node->len)
        {
            goto error;
        }
        if (AC_OK != getOneVal(buf, element_buf, ptr, len))
        {
            goto error;
        }
        if (AC_OK != Type::transStrToData(element_buf, node->data, node->type, i))
        {
            goto error;
        }
    }
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Json::transToJsonStrCore(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len)
{
    if (nullptr == node)
    {
        goto error;
    }
    if (0 != node->name[0])
    {
        addElementWithCheck("\"")
        addElementWithCheck(node->name)
        addElementWithCheck("\"")
        addElementWithCheck(":")
    }

    if (AC_STRUCT == node->type)
    {
        addElementWithCheck("{")
        AcTreeNode *child = node->getFirstChild();

        while (nullptr != child)
        {
            if (child != node->getFirstChild())
            {
                addElementWithCheck(",")
            }
            if (AC_OK != transToJsonStrCore(child, buf, ptr, len))
            {
                goto error;
            }
            child = child->getNeighbor();
        }
        addElementWithCheck("}")
    } else if (AC_ARRAY == node->type)
    {
        addElementWithCheck("[")

        AcTreeNode *child = node->getFirstChild();

        while (nullptr != child)
        {
            if (child != node->getFirstChild())
            {
                addElementWithCheck(",")
            }
            if (AC_OK != transToJsonStrCore(child, buf, ptr, len))
            {
                goto error;
            }
            child = child->getNeighbor();
        }

        addElementWithCheck("]")
    }
    else
    {
        if (AC_OK != addValToJsonStr(node, buf, ptr, len))
        {
            goto error;
        }
    }
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Json::transValToTree(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len)
{
    char element_buf[DATA_BUF_LEN] = {0};

    ptr++;
    if ('\"' != getAndGoToNextSymbol(buf, ptr))
    {
        goto error;
    }
    if (AC_OK != getElement(buf, element_buf, ptr, len))
    {
        goto error;
    }
    if (0 != strncmp("@t", element_buf, DATA_BUF_LEN))
    {
        goto error;
    }
    if (':' != getAndGoToNextSymbol(buf, ptr))
    {
        goto error;
    }
    ptr++;
    if ('\"' != getAndGoToNextSymbol(buf, ptr))
    {
        goto error;
    }
    memset(element_buf, 0, DATA_BUF_LEN);
    if (AC_OK != getElement(buf, element_buf, ptr, len))
    {;
        goto error;
    }
    checkTypeWithTreeNode(node, element_buf);

    if (',' != getAndGoToNextSymbol(buf, ptr))
    {
        goto error;
    }
    ptr++;
    if ('\"' != getAndGoToNextSymbol(buf, ptr))
    {
        goto error;
    }
    memset(element_buf, 0, DATA_BUF_LEN);
    if (AC_OK != getElement(buf, element_buf, ptr, len))
    {

        goto error;
    }
    if (0 != strncmp("@v", element_buf, DATA_BUF_LEN))
    {
        goto error;
    }
    if (':' != getAndGoToNextSymbol(buf, ptr))
    {
        goto error;
    }
    ptr++;
    if ('\"' != getAndGoToNextSymbol(buf, ptr))
    {
        goto error;
    }
    addValueToTreeNode(node, buf, ptr, len);
    if ('}' != getAndGoToNextSymbol(buf, ptr))
    {
        goto error;
    }
    ptr++;
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Json::transArrayToTree(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len)
{
    char element_buf[DATA_BUF_LEN] = {0};
    char next = 0;

    ptr++;
    while ('{' == getAndGoToNextSymbol(buf, ptr))
    {
        ptr++;
        if ('\"' != getAndGoToNextSymbol(buf, ptr))
        {
            goto error;
        }
        if (AC_OK != getElement(buf, element_buf, ptr, len))
        {
            goto error;
        }
        AcTreeNode *child = AcTree::findChildByName(node, element_buf);
        if (nullptr == child)
        {
            goto error;
        }
        if (':' != getAndGoToNextSymbol(buf, ptr))
        {
            goto error;
        }
        ptr++;
        if ('{' != getAndGoToNextSymbol(buf, ptr))
        {
            goto error;
        }
        if (AC_OK != Json::transStructToTree(child, buf, ptr, len))
        {
            goto error;
        }
        next = getAndGoToNextSymbol(buf, ptr);
        if (',' == next)
        {
            ptr++;
        } else if (']' == next)
        {
            break;
        } else
        {
            goto error;
        }
    }
    ptr++;

    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Json::transStructToTree(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len)
{
    char element_buf[DATA_BUF_LEN] = {0};
    char next = 0;
    ptr++;
    while ('\"' == getAndGoToNextSymbol(buf, ptr))
    {
        memset(element_buf, 0, DATA_BUF_LEN);
        if (AC_OK != getElement(buf, element_buf, ptr, len))
        {
            goto error;
        }
        AcTreeNode *child = AcTree::findChildByName(node, element_buf);
        if (nullptr == child)
        {
            goto error;
        }
        if (':' != getAndGoToNextSymbol(buf, ptr))
        {
            goto error;
        }
        ptr++;
        if (AC_STRUCT == child->type)
        {
            if ('{' != getAndGoToNextSymbol(buf, ptr))
            {
                goto error;
            }
            if (AC_OK != transStructToTree(child, buf, ptr, len))
            {
                goto error;
            }
        } else if (AC_ARRAY == child->type)
        {
            if ('[' != getAndGoToNextSymbol(buf, ptr))
            {
                goto error;
            }
            if (AC_OK != transArrayToTree(child, buf, ptr, len))
            {
                goto error;
            }
        } else
        {
            if ('{' != getAndGoToNextSymbol(buf, ptr))
            {
                goto error;
            }
            if (AC_OK != transValToTree(child, buf, ptr, len))
            {
                goto error;
            }
        }
        next = getAndGoToNextSymbol(buf, ptr);
        if (',' == next)
        {
            ptr++;
        } else if ('}' == next)
        {
            break;
        } else
        {
            goto error;
        }
    }
    ptr++;
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Json::transTreeToJsonStr(AcTreeNode *tree, char *buf, uint32_t len)
{
    uint32_t ptr = 0;
    addElementWithCheck("{")
    if (AC_OK != transToJsonStrCore(tree, buf, ptr, len))
    {
        goto error;
    }
    addElementWithCheck("}")
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Json::transJsonStrToTree(AcTreeNode *tree, char *buf, uint32_t len)
{
    uint32_t ptr = 0;
    if (AC_OK != gotoSymbol(buf, ptr, '{'))
    {
        goto error;
    }
    if (AC_STRUCT == tree->type || AC_ARRAY == tree->type)
    {
        if (AC_OK != transStructToTree(tree, buf, ptr, len))
        {
            goto error;
        }
    } else
    {
        if (AC_OK != transValToTree(tree, buf, ptr, len))
        {
            goto error;
        }
    }
    return AC_OK;
    error:
    return AC_ERROR;
}
