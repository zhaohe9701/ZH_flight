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
#include "sys.h"

Imu::Imu()
{
    _printer = new Printer(message_transmit_server->GetQueueHandle());
}

AcTreeNode *Imu::CreateIndex()
{
    AcTreeNode *root = new AcTreeNode();
    AcTreeNode *node = nullptr;

    root->AddData(nullptr, AC_STRUCT, _name, 0);


    node = new AcTreeNode();
    node->AddData((void*)&_get_acc_mark, AC_SWITCH, "get_acc", 1);
    AcTree::AddNode(root, node);

    node = new AcTreeNode();
    node->AddData((void*)&_get_gyr_mark, AC_SWITCH, "get_gyro", 1);
    AcTree::AddNode(root, node);

    node = new AcTreeNode();
    node->AddData((void*)&_get_temp_mark, AC_SWITCH, "get_temperature", 1);
    AcTree::AddNode(root, node);

    node = new AcTreeNode();
    node->AddData((void*)&_id, AC_UINT8, "id", 1);
    AcTree::AddNode(root, node);

    return root;
}


