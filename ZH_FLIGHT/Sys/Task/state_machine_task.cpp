//
// Created by zhaohe on 2023/5/20.
//
#include "aircraft_task.h"
#include "sys.h"

void AircraftTask::StateMachineTask(void *argument)
{
    Condition condition;
    ActionGroup action;

    for (;;)
    {
        /*等待事件触发*/
        AcSignalWait(STATE_MACHINE_SIGNAL);
        /*获取当前事件组合*/
        condition = event_server->GetCurrentCondition();
        /*状态转移*/
        state_machine->TransToNextState(condition);
        /*获得当前状态要采取的动作*/
        action = state_machine->GetAction();
        /*动作交给飞行器执行*/
        aircraft->SetAction(action);
    }
}