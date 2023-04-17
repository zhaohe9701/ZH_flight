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

AcTreeNode *AcTree::FindChildByName(AcTreeNode *parent, char *name)
{
    AcTreeNode *child = nullptr;

    if (nullptr == parent || nullptr == name)
    {
        return nullptr;
    }

    child = parent->_first_child;
    while (nullptr != child)
    {
        if (0 == strncmp(child->name, name, PARAM_NAME_LEN))
        {
            return child;
        }
        child = child->_neighbor;
    }
    return nullptr;
}



