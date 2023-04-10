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
#include <string.h>

AcTreeNode *AcTreeNode::GetFirstChild()
{
    return _first_child;
}

AcTreeNode *AcTreeNode::GetNeighbor()
{
    return _neighbor;
}

void AcTreeNode::AddData(void *data, AC_DATA_TYPE type, char *name, uint16_t len)
{
    this->data = data;
    this->type = type;
    strncpy(this->name, name, PARAM_NAME_LEN);
}

AcTree::AcTree()
{
    _root = new AcTreeNode();
}

static AcTreeNode *FindNodeCore(AcTreeNode *node, char *uri, int ptr)
{
    int name_ptr = 0;
    int uri_ptr = ptr;
    while (uri[uri_ptr] != 0 && uri[uri_ptr] != '/')
    {
        if (uri[uri_ptr] != node->name[name_ptr])
        {
            return FindNodeCore(node->GetNeighbor(), uri, ptr);
        }
        uri_ptr++;
        name_ptr++;
    }
    if(uri[uri_ptr] == 0 && node->name[name_ptr] == 0)
    {
        return node;
    }
    else
    {
        uri_ptr++;
        return FindNodeCore(node->GetFirstChild(), uri, uri_ptr);
    }
}

AcTreeNode *AcTree::FindNode(AcTree *tree, char *uri)
{
    return FindNodeCore(tree->GetRoot(), uri, 0);
}

AcTreeNode *AcTree::GetRoot()
{
    return _root;
}

void AcTree::AddNode(AcTreeNode *node)
{
    AcTreeNode *child = nullptr;

    child = node->GetFirstChild();
    if (nullptr != child)
    {
        child = child->GetNeighbor();
    }
    child->_neighbor = node;
}
