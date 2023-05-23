//
// Created by zhaohe on 2023/4/20.
//
#include "baro.h"

AcTreeNode *Baro::CreateIndex()
{
    AcTreeNode *root = new AcTreeNode();
    AcTreeNode *node = nullptr;

    root->AddData(nullptr, AC_STRUCT, _name, 0);


    node = new AcTreeNode();
    node->AddData((void*)&_get_altitude_mark, AC_SWITCH, "get_altitude", 1);
    AcTree::AddNode(root, node);

    node = new AcTreeNode();
    node->AddData((void*)&_get_pressure_mark, AC_SWITCH, "get_pressure", 1);
    AcTree::AddNode(root, node);

    node = new AcTreeNode();
    node->AddData((void*)&_get_temp_mark, AC_SWITCH, "get_temperature", 1);
    AcTree::AddNode(root, node);

    node = new AcTreeNode();
    node->AddData((void*)&_id, AC_UINT8, "id", 1);
    AcTree::AddNode(root, node);

//    node = new AcTreeNode();
//    node->AddData((void*)&_test, AC_UINT8, "test_array", 4);
//    AcTree::AddNode(root, node);

    return root;
}

