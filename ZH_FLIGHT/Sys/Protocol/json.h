/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-13 23:12:45
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-13 23:48:02
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\json.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __JSON_H__
#define __JSON_H__

#include "cjson.h"

class Json
{
public:
    Json();
    void AddNum(const char *name, const double number);
    void AddString(const char *name, const char *string);
    void Addbool(const char *name, const bool value);
    void AddJson(const char *name, const Json &cjson);
    char* Serialize();
private:
    cJSON* _cjson = nullptr;

};

#endif