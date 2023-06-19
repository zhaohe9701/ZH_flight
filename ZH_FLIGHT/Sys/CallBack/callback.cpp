#include "main.h"
#include "stm32h7xx_hal.h"
#include "sys.h"
#include "config.h"
#include "string.h"

#define MAX_USB_RECEIVE 64

void ExitCallBack(uint16_t GPIO_Pin);
void UsbCallBackHandle(uint8_t *buf, uint32_t len);

extern "C"
{
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    ExitCallBack(GPIO_Pin);
}
}

extern "C"
{
void UsbCallback(uint8_t *buf, uint32_t len)
{
    UsbCallBackHandle(buf, len);
}
}

void ExitCallBack(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == IMU1_EXIT_PIN)
    {
        if (nullptr != imu_sem)
        {
            imu_sem->give();
        }
    }
}

static uint32_t ptr = 0;
static uint8_t usb_buf[MAX_USB_BUF_LEN] = {0};
void UsbCallBackHandle(uint8_t *buf, uint32_t len)
{
    if (ptr + len > MAX_USB_BUF_LEN)
    {
        memset(usb_buf, 0, MAX_USB_BUF_LEN);
        ptr = 0;
        return;
    }
    memcpy(usb_buf + ptr, buf, len);
    ptr = ptr + len;
    if (len != MAX_USB_RECEIVE || buf[MAX_USB_RECEIVE - 1] == '\n')
    {
        Message message;
        message.buf = usb_buf;
        message.len = ptr;
        message.src_port = USB_PORT_NUMBER;
        (message_receive_server->getMessageManager())->transmit(message);
        memset(usb_buf, 0, MAX_USB_BUF_LEN);
        ptr = 0;
    }
}