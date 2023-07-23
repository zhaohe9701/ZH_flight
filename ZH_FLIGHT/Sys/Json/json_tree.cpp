//
// Created by zhaohe on 2023/7/22.
//
#include <string.h>
#include "json_tree.h"
#include "sys.h"

#define appendString(buf, str, ptr, len)                \
        {                                               \
        uint32_t l = strlen(str);                       \
        if (l + (ptr) + 1 > (len))                      \
        {                                               \
            goto error;                                 \
        }                                               \
        strncpy((buf) + (ptr), str, (len) - (ptr) - 1); \
        (ptr) += l;                                     \
        }                                               \

#define gotoSymbol(buf, ptr, symbol)                    \
        {                                               \
        while((buf)[ptr] != (symbol))                   \
        {                                               \
            if (0 == (buf)[ptr])                        \
            {                                           \
                goto error;                             \
            }                                           \
            (ptr)++;                                    \
        }                                               \
        }                                               \

#define gotoNextSymbol(buf, ptr)                        \
        {                                               \
        (ptr)++;                                        \
        while (' ' == (buf)[ptr])                       \
        {                                               \
            (ptr)++;                                    \
        }                                               \
        }                                               \

#define getElement(buf, ptr, element, len)                              \
        {                                                               \
        uint32_t i = 0;                                                 \
        while ('\"' != (buf)[ptr] && 0 != (buf)[ptr] && i < (len) - 1)  \
        {                                                               \
            (element)[i] = (buf)[ptr];                                  \
            i++;                                                        \
            (ptr)++;                                                    \
        }                                                               \
        }                                                               \

#define getUrl(buf, ptr, element, len)                                  \
        {                                                               \
        uint32_t i = 0;                                                 \
        while ('/' != (buf)[ptr] && 0 != (buf)[ptr] && i < (len) - 1)   \
        {                                                               \
            (element)[i] = (buf)[ptr];                                  \
            i++;                                                        \
            (ptr)++;                                                    \
        }                                                               \
        }                                                               \


#define skipSpace(buf, ptr)                                             \
        {                                                               \
        while (' ' == (buf)[ptr])                                       \
        {                                                               \
            (ptr)++;                                                    \
        }                                                               \
        }                                                               \


JsonTree *JsonTree::getFirstChild()
{
    return _first_child;
}

JsonTree *JsonTree::getNeighbor()
{
    return _neighbor;
}

JsonTree *JsonTree::findChild(char *child_name)
{
    JsonTree *temp_child = _first_child;
    while (nullptr != temp_child)
    {
        if (0 == strcmp(temp_child->name, child_name))
        {
            return temp_child;
        }
        temp_child = temp_child->_neighbor;
    }
    return nullptr;
}

JsonTree *JsonTree::findNode(char *url, uint32_t &ptr)
{
    char temp_url[PARAM_NAME_LEN] = {0};
    JsonTree *child = nullptr;

    getUrl(url, ptr, temp_url, PARAM_NAME_LEN);
    debug_printer->info("url:%s\n", temp_url);
    if ('/' == url[ptr])
    {
        child = findChild(temp_url);
        if (nullptr == child)
        {
            goto error;
        } else
        {
            gotoNextSymbol(url, ptr);
            return child->findNode(url, ptr);
        }
    } else
    {
        child = findChild(temp_url);
        if (nullptr == child)
        {
            goto error;
        } else
        {
            return child;
        }
    }
error:
    return nullptr;
}

void JsonTree::addChild(JsonTree *tree)
{
    JsonTree *temp_node = _first_child;

    if (nullptr == _first_child)
    {
        _first_child = tree;
        return;
    }
    while (temp_node->_neighbor)
    {
        temp_node = temp_node->_neighbor;
    }
    temp_node->_neighbor = tree;
}


void JsonTree::addData(void *in_data, AC_DATA_TYPE in_type, const char *in_name)
{
    data = in_data;
    type = in_type;
    strncpy(name, in_name, PARAM_NAME_LEN);
}

AC_RET JsonTree::toString(char *buf, uint32_t &ptr, uint32_t len, AC_DATA_TYPE parent_type)
{
    if (AC_ARRAY != parent_type)
    {
        appendString(buf, "\"", ptr, len);
        appendString(buf, name, ptr, len);
        appendString(buf, "\":", ptr, len);
    }
    if (AC_STRUCT == type)
    {
        if (nullptr == _first_child)
        {
            return AC_OK;
        }
        appendString(buf, "{", ptr, len);
        if (AC_OK != _first_child->toString(buf, ptr, len, type))
        {
            goto error;
        }
        appendString(buf, "}", ptr, len);
    }   else if (AC_ARRAY == type)
    {
        if (nullptr == _first_child)
        {
            return AC_OK;
        }
        appendString(buf, "[", ptr, len);
        if (AC_OK != _first_child->toString(buf, ptr, len, type))
        {
            goto error;
        }
        appendString(buf, "]", ptr, len);
    } else
    {
        char data_str[DATA_BUF_LEN] = {0};
        appendString(buf, "\"", ptr, len);
        if (AC_OK != Type::transDataToStr(data_str, data, type))
        {
            goto error;
        }
        appendString(buf, data_str, ptr, len);
        appendString(buf, "\"", ptr, len);
    }
    if (AC_ROOT == parent_type)
    {
        return AC_OK;
    }
    if (nullptr == _neighbor)
    {
        return AC_OK;
    }
    appendString(buf, ",", ptr, len);
    if (AC_OK != _neighbor->toString(buf, ptr, len, parent_type))
    {
        goto error;
    }
    return AC_OK;
error:
    return AC_ERROR;
}

AC_RET toCapability(char *buf, uint32_t &ptr, uint32_t len, AC_DATA_TYPE parent_type)
{

}

AC_RET JsonTree::toBin(uint8_t *bin, uint32_t &ptr, uint32_t len)
{
    return AC_OK;
}

AC_RET JsonTree::fromString(char *buf, uint32_t &ptr)
{
    debug_printer->info("NAME:%s\n", name);
    skipSpace(buf, ptr);
    if (AC_STRUCT == type)
    {
        if ('{' != buf[ptr])
        {
            goto error;
        }

        do
        {
            gotoNextSymbol(buf, ptr);
            if ('\"' != buf[ptr])
            {
                goto error;
            }
            ptr++;
            char temp_name[PARAM_NAME_LEN] = {0};
            getElement(buf, ptr, temp_name, PARAM_NAME_LEN);
            JsonTree *temp_child = findChild(temp_name);
            if (nullptr == temp_child)
            {
                goto error;
            }
            gotoNextSymbol(buf, ptr);
            if (':' != buf[ptr])
            {
                goto error;
            }
            gotoNextSymbol(buf, ptr);
            if (AC_OK != temp_child->fromString(buf, ptr))
            {
                goto error;
            }
        } while (',' == buf[ptr]);
        skipSpace(buf, ptr);
        if ('}' != buf[ptr])
        {
            goto error;
        }
        gotoNextSymbol(buf, ptr);
    } else if (AC_ARRAY == type)
    {
        if ('[' != buf[ptr])
        {
            goto error;
        }
        JsonTree *temp_child = _first_child;
        do
        {
            gotoNextSymbol(buf, ptr);
            if (AC_OK != temp_child->fromString(buf, ptr))
            {
                goto error;
            }
            temp_child = temp_child->_neighbor;
        } while (',' == buf[ptr] && nullptr != temp_child);
        skipSpace(buf, ptr);
        if (']' != buf[ptr])
        {
            goto error;
        }
        gotoNextSymbol(buf, ptr);
    } else
    {
        if ('\"' != buf[ptr])
        {
            goto error;
        }
        ptr++;
        char value[DATA_BUF_LEN] = {0};
        getElement(buf, ptr, value, DATA_BUF_LEN);
        Type::transStrToData(value, data, type);
        gotoNextSymbol(buf, ptr);
    }
    return AC_OK;
error:
    return AC_ERROR;
}

AC_RET JsonTree::fromBin(uint8_t *buf)
{
    return AC_OK;
}

JsonTree *JsonTree::findNode(char *url)
{
    uint32_t ptr = 0;
    char temp_url[PARAM_NAME_LEN] = {0};
    if ('/' != url[ptr])
    {
        return nullptr;
    }
    gotoNextSymbol(url, ptr);
    getUrl(url, ptr, temp_url, PARAM_NAME_LEN);
    if (0 == strcmp(temp_url, name))
    {
        if ('/' != url[ptr])
        {
            return this;
        }
    } else
    {
        return nullptr;
    }
    gotoNextSymbol(url, ptr);
    return findNode(url, ptr);
}

AC_RET JsonTree::toString(char *buf, uint32_t len)
{
    uint32_t ptr = 0;
    appendString(buf, "{", ptr, len);
    if (AC_OK != toString(buf, ptr, len, AC_ROOT))
    {
        goto error;
    }
    appendString(buf, "}", ptr, len);
    return AC_OK;
error:
    return AC_ERROR;
}

AC_RET JsonTree::fromString(char *buf)
{
    uint32_t  ptr = 0;
    if (AC_OK != fromString(buf, ptr))
    {
        return AC_ERROR;
    }
    return AC_OK;
}