/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-08 23:04:24
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-10 23:54:16
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
    void AddData(void *in_data, AC_DATA_TYPE in_type, char *name_in, uint16_t in_len);
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
    AcTree(void *in_data, AC_DATA_TYPE in_type, char *in_name, uint16_t in_len);
    static AcTreeNode *FindNode(AcTree *tree, char *uri);
    AcTreeNode *GetRoot();
    void AddNode(AcTreeNode *node);
    ~AcTree();
};

#endif