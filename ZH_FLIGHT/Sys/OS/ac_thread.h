//
// Created by zhaohe on 2023/5/9.
//

#ifndef __AC_THREAD_H__
#define __AC_THREAD_H__

#include "os.h"
#include "type.h"
//#include "ac_list.h"

class AcThread
{
public:
    AC_RET Init(AcFunction func, const char *name, uint16_t stack, AcPriority prio, void *param = nullptr);
    void Suspend();
    AC_RET Resume();
    void Kill();

//    static AcList<AcThread> thread_list;
//    static void AddTreadToList(AcThread *thread);
private:
    ThreadHandle _handle = nullptr;
};


#endif //__AC_THREAD_H__
