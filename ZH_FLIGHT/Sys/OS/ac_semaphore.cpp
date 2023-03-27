/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-26 22:28:07
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 22:42:00
 * @FilePath: \ZH_FLIGHT\Sys\OS\ac_semaphore.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "ac_semaphore.h"
#include "cmsis_os2.h"
#include "type.h"

AcSemaphore::AcSemaphore()
{   
    _handle = osSemaphoreNew(1, 1, NULL);
}

AC_RET AcSemaphore::Give()
{
    if (osOK != osSemaphoreRelease(_handle))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET AcSemaphore::Get()
{
    if (osOK != osSemaphoreAcquire(_handle, osWaitForever))
    {
        return AC_ERROR;
    }
    return AC_OK;
}