/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-08 23:04:24
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-12 19:50:25
 * @FilePath: \ZH_FLIGHT\Sys\Common\ac_tree.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __AC_TREE_H__
#define __AC_TREE_H__

#include "type.h"

class AcTreeNode
{
friend class AcTree;
private:
    AcTreeNode *_first_child = nullptr;
    AcTreeNode *_neighbor = nullptr;
public:
    AcTreeNode *GetFirstChild();
    AcTreeNode *GetNeighbor();
    char name[PARAM_NAME_LEN] = {0};
    AC_DATA_TYPE type;
    uint16_t len;
    void *data;
};

class AcTree
{
private:
    AcTreeNode *_root = nullptr;
public:
    AcTree();
    static AcTreeNode *FindNode(AcTree *tree, char *uri);
    AcTreeNode *GetRoot();
    void AddNode(AcTreeNode *node);
    ~AcTree();
};

#endif