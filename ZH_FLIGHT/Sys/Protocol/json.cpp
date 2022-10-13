/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-13 23:35:40
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-13 23:49:09
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\json.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "json.h"

Json::Json()
{
    _cjson = cJSON_CreateObject();
}

void Json::Addbool(const char *name, bool value)
{
    if (value == true)
    {
        cJSON_AddTrueToObject(_cjson, name);
    }
    else
    {
        cJSON_AddFalseToObject(_cjson, name);
    }
}

void Json::AddNum(const char *name, double number)
{
    cJSON_AddNumberToObject(_cjson, name, number);
}

void Json::AddString(const char *name, const char *string)
{
    cJSON_AddStringToObject(_cjson, name, string);
}

void Json::AddJson(const char *name, const Json &cjson)
{
    cJSON_AddItemToObject(_cjson, name, cjson._cjson);
}

char* Json::Serialize()
{
    return cJSON_Print(_cjson);
}