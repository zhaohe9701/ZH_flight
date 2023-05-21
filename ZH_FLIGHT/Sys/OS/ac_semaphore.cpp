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
#include "sys.h"

AcSemaphore::AcSemaphore()
{
    _handler = xSemaphoreCreateBinary();
    xSemaphoreGive (_handler);
}

AC_RET AcSemaphore::Give()
{
    if (IS_IN_IRQ())
    {
        BaseType_t yield = pdFALSE;
        if (pdTRUE != xSemaphoreGiveFromISR(_handler, &yield))
        {
            // debug_printer->Error("IRQ\r\n");
            return AC_ERROR;
        } else
        {
            // debug_printer->Info("IRQ\r\n");
            portYIELD_FROM_ISR(yield);
        }
    } else
    {
        if (pdTRUE != xSemaphoreGive(_handler))
        {
            // debug_printer->Error("NORMAL\r\n");
            return AC_ERROR;
        }
        // debug_printer->Info("NORMAL\r\n");
    }
    return AC_OK;
}

AC_RET AcSemaphore::Get(uint32_t timeout)
{
    if (IS_IN_IRQ())
    {
        BaseType_t yield = pdFALSE;
        if (pdTRUE != xSemaphoreTakeFromISR(_handler, &yield))
        {
            return AC_ERROR;
        } else
        {
            portYIELD_FROM_ISR(yield);
        }
    } else
    {
        if (pdTRUE != xSemaphoreTake(_handler, (TickType_t)timeout))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}