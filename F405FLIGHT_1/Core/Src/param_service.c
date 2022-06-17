#include "param_service.h"
#include "pid.h"

void Param_Service(union ParamTrans *param)
{
    if(param->chr[0] == PID_PARAM)
    {
        if(1 == Update_Pid_Param(param))
        {
            usb_printf("Pid Param Update Error!\r\n");
        }
        else
        {
            usb_printf("Pid Param Update Succeed!\r\n");
        }
    }
}