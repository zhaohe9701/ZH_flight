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
    // debug_printer->info("name:%s uri:%s\n", node->name, uri);
    while (uri[uri_ptr] != 0 && uri[uri_ptr] != '/')
    {
        if (uri[uri_ptr] != node->name[name_ptr])
        {
            return FindNodeCore(node->getNeighbor(), uri, ptr);
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
        return FindNodeCore(node->getFirstChild(), uri, uri_ptr);
    }
}

AcTreeNode *AcTreeNode::getFirstChild()
{
    return _first_child;
}

AcTreeNode *AcTreeNode::getNeighbor()
{
    return _neighbor;
}

void AcTreeNode::addData(void *in_data, AC_DATA_TYPE in_type, const char *in_name, uint16_t in_len)
{
    data = in_data;
    type = in_type;
    strncpy(name, in_name, PARAM_NAME_LEN);
    len = in_len;
}

AcTreeNode *AcTree::findNode(AcTreeNode *tree, char *uri)
{
    return FindNodeCore(tree, uri, 0);
}

void AcTree::addNode(AcTreeNode* root, AcTreeNode *node)
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

AcTreeNode *AcTree::findChildByName(AcTreeNode *parent, char *name)
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



