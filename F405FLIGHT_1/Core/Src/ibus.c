#include "ibus.h"
#include "system.h"
uint16_t ibus_channel[18] = {0};
extern volatile enum Mutex remote_cmd_mutex;

uint8_t remote_ind = 0;

uint8_t Ibus_Parse(uint8_t *ibus_data)
{
    uint16_t CH[18] = {0};
    remote_ind++;
    if(ibus_data[0] == 0x20 && ibus_data[1] == 0x40)
    {
        CH[0]   = ((uint16_t)(ibus_data[3]   & 0x0F) << 8) | ibus_data[2];
        CH[1]   = ((uint16_t)(ibus_data[5]   & 0x0F) << 8) | ibus_data[4];
        CH[2]   = ((uint16_t)(ibus_data[7]   & 0x0F) << 8) | ibus_data[6];
        CH[3]   = ((uint16_t)(ibus_data[9]   & 0x0F) << 8) | ibus_data[8];
        CH[4]   = ((uint16_t)(ibus_data[11]  & 0x0F) << 8) | ibus_data[10];
        CH[5]   = ((uint16_t)(ibus_data[13]  & 0x0F) << 8) | ibus_data[12];
        CH[6]   = ((uint16_t)(ibus_data[15]  & 0x0F) << 8) | ibus_data[14];
        CH[7]   = ((uint16_t)(ibus_data[17]  & 0x0F) << 8) | ibus_data[16];
        CH[8]   = ((uint16_t)(ibus_data[19]  & 0x0F) << 8) | ibus_data[18];
        CH[9]   = ((uint16_t)(ibus_data[21]  & 0x0F) << 8) | ibus_data[20];
        CH[10]  = ((uint16_t)(ibus_data[23]  & 0x0F) << 8) | ibus_data[22];
        CH[11]  = ((uint16_t)(ibus_data[25]  & 0x0F) << 8) | ibus_data[24];

        CH[12]  = ((uint16_t)ibus_data[27]  <<  8) | ibus_data[26];
        CH[13]  = ((uint16_t)ibus_data[29]  <<  8) | ibus_data[28];

        if(remote_cmd_mutex == UNLOCK)
        {
            remote_cmd_mutex = LOCK;
            for(int i = 0;i < 18; ++i)
            {
                ibus_channel[i] = CH[i];
            }
            remote_cmd_mutex = UNLOCK;
        }
        //usb_printf("%d,%d,%d,%d,%d,%d\r\n",CH[0],CH[1],CH[2],CH[3],CH[4],CH[5]);

    }
    else
    {
        return 1;
    }
    return 0;
}