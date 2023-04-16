/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-09 00:10:07
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-11 00:11:59
 * @FilePath: \ZH_FLIGHT\Sys\Common\ac_tree.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "ac_tree.h"
#include "sys.h"
#include <cstring>
#include <cstdio>

#define TYPE_BUF_LEN 16
#define DATA_BUF_LEN 64

#define AddElementWithCheck(element)                \
{                                                   \
if (AC_OK != AddElement(buf, (element), ptr, len))  \
    goto error;                                     \
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

inline static AC_RET MatchType(char *type_buf, AC_DATA_TYPE type)
{
    switch (type)
    {
        case AC_UINT8:
            strncpy(type_buf, "uint8", TYPE_BUF_LEN - 1);
            break;
        case AC_UINT16:
            strncpy(type_buf, "uint16", TYPE_BUF_LEN - 1);
            break;
        case AC_UINT32:
            strncpy(type_buf, "uint32", TYPE_BUF_LEN - 1);
            break;
        case AC_INT8:
            strncpy(type_buf, "int8", TYPE_BUF_LEN - 1);
            break;
        case AC_INT16:
            strncpy(type_buf, "int16", TYPE_BUF_LEN - 1);
            break;
        case AC_INT32:
            strncpy(type_buf, "int32", TYPE_BUF_LEN - 1);
            break;
        case AC_FLOAT:
            strncpy(type_buf, "float", TYPE_BUF_LEN - 1);
            break;
        case AC_DOUBLE:
            strncpy(type_buf, "double", TYPE_BUF_LEN - 1);
            break;
        case AC_STRING:
            strncpy(type_buf, "string", TYPE_BUF_LEN - 1);
            break;
        case AC_SWITCH:
            strncpy(type_buf, "switch", TYPE_BUF_LEN - 1);
            break;
        default:
            return AC_ERROR;
    }
    return AC_OK;
}

inline static AC_RET TransDataToStr(char *data_buf, AcTreeNode *node, uint16_t index)
{
    switch (node->type)
    {
        case AC_UINT8:
            snprintf(data_buf, DATA_BUF_LEN, "%u", ((uint8_t *) node->data)[index]);
            break;
        case AC_UINT16:
            snprintf(data_buf, DATA_BUF_LEN, "%u", ((uint16_t *) node->data)[index]);
            break;
        case AC_UINT32:
            snprintf(data_buf, DATA_BUF_LEN, "%lu", ((uint32_t *) node->data)[index]);
            break;
        case AC_INT8:
            snprintf(data_buf, DATA_BUF_LEN, "%d", ((int8_t *) node->data)[index]);
            break;
        case AC_INT16:
            snprintf(data_buf, DATA_BUF_LEN, "%d", ((int16_t *) node->data)[index]);
            break;
        case AC_INT32:
            snprintf(data_buf, DATA_BUF_LEN, "%ld", ((int32_t *) node->data)[index]);
            break;
        case AC_FLOAT:
            snprintf(data_buf, DATA_BUF_LEN, "%f", ((float *) node->data)[index]);
            break;
        case AC_DOUBLE:
            snprintf(data_buf, DATA_BUF_LEN, "%f", ((double *)node->data)[index]);
            break;
        case AC_SWITCH:
            if (AC_OFF == ((AcSwitch *)node->data)[index])
            {
                strcpy(data_buf, "off");
            }
            else
            {
                strcpy(data_buf, "on");
            }
            break;
        case AC_STRING:
            if (index != 0)
            {
                return AC_ERROR;
            }
            snprintf(data_buf, DATA_BUF_LEN, "%s", (char*)node->data);
            break;
        default:
            return AC_ERROR;
    }
    return AC_OK;
}

static AcTreeNode *FindNodeCore(AcTreeNode *node, char *uri, int ptr)
{
    int name_ptr = 0;
    int uri_ptr = ptr;

    if (nullptr == node)
    {
        return nullptr;
    }
    debug_printer->Info("name:%s uri:%s\n", node->name, uri);
    osDelay(10);
    while (uri[uri_ptr] != 0 && uri[uri_ptr] != '/')
    {
        if (uri[uri_ptr] != node->name[name_ptr])
        {
            return FindNodeCore(node->GetNeighbor(), uri, ptr);
        }
        uri_ptr++;
        name_ptr++;
    }
    if (uri[uri_ptr] == 0 && node->name[name_ptr] == 0)
    {
        return node;
    } else
    {
        uri_ptr++;
        return FindNodeCore(node->GetFirstChild(), uri, uri_ptr);
    }
}

static AC_RET AddValToJsonStr(AcTreeNode *node, char *buf, uint32_t &ptr, uint32_t len, uint16_t index)
{
    char type_buf[TYPE_BUF_LEN] = {0};
    char data_buf[DATA_BUF_LEN] = {0};
    AddElementWithCheck("{");
    AddElementWithCheck("\"@t\":");
    if (AC_OK != MatchType(type_buf, node->type))
    {
        goto error;
    }
    AddElementWithCheck("\"");
    AddElementWithCheck(type_buf);
    AddElementWithCheck("\"");

    AddElementWithCheck(",");
    AddElementWithCheck("\"@v\":");
    if (AC_OK != TransDataToStr(data_buf, node, index))
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
    AddElementWithCheck("\"");
    AddElementWithCheck(node->name);
    AddElementWithCheck("\"");
    AddElementWithCheck(":");
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

AcTreeNode *AcTreeNode::GetFirstChild()
{
    return _first_child;
}

AcTreeNode *AcTreeNode::GetNeighbor()
{
    return _neighbor;
}

void AcTreeNode::AddData(void *in_data, AC_DATA_TYPE in_type, const char *in_name, uint16_t in_len)
{
    data = in_data;
    type = in_type;
    strncpy(name, in_name, PARAM_NAME_LEN);
    len = in_len;
}

AcTreeNode *AcTree::FindNode(AcTreeNode *tree, char *uri)
{
    return FindNodeCore(tree, uri, 0);
}

void AcTree::AddNode(AcTreeNode* root, AcTreeNode *node)
{
    AcTreeNode *child = nullptr;

    child = root->_first_child;
    if (nullptr == child)
    {
        root->_first_child = node;
        return;
    }
    while (nullptr != child->_neighbor)
    {
        child = child->_neighbor;
    }
    child->_neighbor = node;
}


AC_RET AcTree::TransToJsonStr(AcTreeNode *tree, char *buf, uint32_t len)
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
