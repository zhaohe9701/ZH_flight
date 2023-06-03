/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-03 02:34:04
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-22 23:07:42
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\sensor.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "sensor.h"
#include "sys.h"

void Sensor::addImu(Imu *imu)
{
    this->imu = imu;
}

void Sensor::addBaro(Baro *baro)
{
    this->baro = baro;
}

void Sensor::init()
{
    if (nullptr != imu)
    {
        imu->init();
    }
    if (nullptr != baro)
    {
        baro->init();
    }
}

AcTreeNode *Sensor::createIndex()
{
    AcTreeNode *root = new AcTreeNode();
    AcTreeNode *node = nullptr;

    root->addData(nullptr, AC_STRUCT, "sensor", 0);

    node = imu->CreateIndex();
    AcTree::addNode(root, node);

    node = baro->createIndex();
    AcTree::addNode(root, node);

    return root;
}
