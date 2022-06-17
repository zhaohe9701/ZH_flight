#ifndef _UPPER_COMPUTER_H__
#define _UPPER_COMPUTER_H__

#include "main.h"
#include "flight.h"
#define UPPER_RECEIVE_BUFF_SIZE 24
#define UPPER_SEND_BUFF_SIZE    24

#define STATE_TO_UPPER          0x40

#define UPPER_COMMAND           0x8C
#define ATTITUDE_COMMAND        0xF0


// union UpperCommandTrans
// {
//     uint8_t chr[32];
//     float   flo[8];
// };

// union SendStateTrans
// {
//     uint8_t chr[48];
//     float   flo[12];
// };

void Upper_Computer_Receive_Parse(uint8_t *upper_receive_buff);
void Send_State_To_Upper_Computer(State *actual_state, uint8_t rate);

#endif