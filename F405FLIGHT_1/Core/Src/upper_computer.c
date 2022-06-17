#include "upper_computer.h"
#include "system.h"

extern volatile enum Mutex upper_cmd_mutex;
extern UART_HandleTypeDef huart3;
union ParamTrans upper_command_raw;

void Upper_Computer_Receive_Parse(uint8_t *upper_receive_buff)
{
    if(upper_receive_buff[0] == UPPER_COMMAND && upper_receive_buff[1] == ATTITUDE_COMMAND)
    {
        if(upper_cmd_mutex == UNLOCK)
        {
            upper_cmd_mutex = LOCK;
            for(int i = 0; i < UPPER_RECEIVE_BUFF_SIZE; ++i)
            {
                upper_command_raw.chr[i] = upper_receive_buff[i];
            }
            upper_cmd_mutex = UNLOCK;
        }
    }
}

void Send_State_To_Upper_Computer(State *actual_state, uint8_t rate)
{
    static uint8_t count = 0;
    if(count < rate)
    {
        count++;
        return;
    }
    count = 0;
    static union ParamTrans state_data;
    state_data.flo[1] = actual_state->pitch;
    state_data.flo[2] = actual_state->roll;
    state_data.flo[3] = actual_state->yaw;
    // state_data.flo[4] = actual_state->pitch_rate;
    // state_data.flo[5] = actual_state->roll_rate;
    // state_data.flo[6] = actual_state->yaw_rate;
    // state_data.flo[7] = actual_state->acc_x;
    // state_data.flo[8] = actual_state->acc_y;
    // state_data.flo[9] = actual_state->acc_z;
    state_data.flo[4] = 0.0f;
    state_data.flo[5] = 0.0f;
    state_data.chr[0] = STATE_TO_UPPER;
    state_data.chr[1] = 0xE0;
    state_data.chr[2] = 0x00;
    state_data.chr[3] = 0x00;
    HAL_UART_Transmit_DMA(&huart3, state_data.chr, UPPER_SEND_BUFF_SIZE);
}