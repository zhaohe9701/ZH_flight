//
// Created by zhaohe on 2023/4/20.
//
#include "baro.h"

JsonTree *Baro::createIndex()
{
    JsonTree *root = new JsonTree();
    JsonTree *node = nullptr;

    root->addData(nullptr, AC_STRUCT, _name);


    node = new JsonTree();
    node->addData((void *) &_get_altitude_mark, AC_SWITCH, "get_altitude");
    root->addChild(node);

    node = new JsonTree();
    node->addData((void *) &_get_pressure_mark, AC_SWITCH, "get_pressure");
    root->addChild(node);

    node = new JsonTree();
    node->addData((void *) &_get_temp_mark, AC_SWITCH, "get_temperature");
    root->addChild(node);

    node = new JsonTree();
    node->addData((void *) &_id, AC_UINT8, "id");
    root->addChild(node);

    JsonTree *array = new JsonTree();
    array->addData(nullptr, AC_ARRAY, "test_array");

    node = new JsonTree();
    node->addData((void*)&_test[0], AC_UINT8, "1");
    array->addChild(node);
    node = new JsonTree();
    node->addData((void*)&_test[1], AC_UINT8, "2");
    array->addChild(node);
    node = new JsonTree();
    node->addData((void*)&_test[2], AC_UINT8, "3");
    array->addChild(node);
    node = new JsonTree();
    node->addData((void*)&_test[3], AC_UINT8, "4");
    array->addChild(node);

    root->addChild(array);
//    node = new AcTreeNode();
//    node->addData((void*)&_test, AC_UINT8, "test_array", 4);
//    AcTree::addNode(root, node);

    return root;
}

