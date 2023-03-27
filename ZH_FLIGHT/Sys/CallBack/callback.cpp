#include "main.h"
#include "stm32h7xx_hal.h"
#include "sys.h"
#include "config.h"
#include "string.h"

#define MAX_USB_RECEIVE 64

void ExitCallBack(uint16_t GPIO_Pin);
void UsbCallBack(uint8_t *buf, uint32_t len);

extern "C"
{
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    ExitCallBack(GPIO_Pin);
}
}

extern "C"
{
void Usb_Callback(uint8_t *buf, uint32_t len)
{
    UsbCallBack(buf, len);
}
}

void ExitCallBack(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == IMU1_EXIT_PIN)
    {
        if (nullptr != imu_sem)
        {
            imu_sem->Give();
        }
    }
}

static Message receive_message;
static uint32_t ptr = 0;

void UsbCallBack(uint8_t *buf, uint32_t len)
{
    if (ptr + len > MAX_MESSAGE_LENGTH)
    {
        //UsbPrintf("1 %d\n", len);
        memcpy(receive_message.data + ptr, buf, MAX_MESSAGE_LENGTH - ptr);
        receive_message.length = MAX_MESSAGE_LENGTH;
        (message_receive_server->GetQueueHandle())->Push(&receive_message);
        memset(receive_message.data + 1, 0, MAX_MESSAGE_LENGTH);
        memcpy(receive_message.data + 1, buf + MAX_MESSAGE_LENGTH - ptr, len - (MAX_MESSAGE_LENGTH - ptr));
        ptr = len - (MAX_MESSAGE_LENGTH - ptr);
        return;
    }
    if (len < MAX_USB_RECEIVE)
    {
        UsbPrintf("2 %d\n", len);
        memcpy(receive_message.data + ptr, buf, len);
        receive_message.length = ptr + len;
        (message_receive_server->GetQueueHandle())->Push(&receive_message);
        ptr = 0;
        memset(receive_message.data, 0, MAX_MESSAGE_LENGTH);
    }
    else
    {
        //UsbPrintf("3 %d\n", len);
        memcpy(receive_message.data + ptr, buf, len);
        ptr += len;
        if ('\n' == buf[MAX_MESSAGE_LENGTH])
        {
            receive_message.length = ptr;
            (message_receive_server->GetQueueHandle())->Push(&receive_message);
            ptr = 0;
            memset(receive_message.data, 0, MAX_MESSAGE_LENGTH);
        }
    }
}