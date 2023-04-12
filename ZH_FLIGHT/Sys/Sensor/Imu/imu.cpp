/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-04-10 22:48:12
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-11 00:34:12
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\imu.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "imu.h"


AcTreeNode *Imu::CreateIndex()
{
    AcTree *tree = new AcTree(nullptr, AC_STRUCT, (char *)"imu", 0);
    AcTreeNode *node = nullptr;

    node = new AcTreeNode();
    node->AddData((void*)&_get_acc_mark, AC_MARK, (char*)"get_acc", 1);
    tree->AddNode(node);

    node = new AcTreeNode();
    node->AddData((void*)&_get_gyr_mark, AC_MARK, (char*)"get_gyro", 1);
    tree->AddNode(node);

    node = new AcTreeNode();
    node->AddData((void*)&_get_temp_mark, AC_MARK, (char*)"get_temperature", 1);
    tree->AddNode(node);

    node = new AcTreeNode();
    node->AddData((void*)&_get_id_mark, AC_MARK, (char*)"get_id", 1);
    tree->AddNode(node);

    return tree->GetRoot();
}