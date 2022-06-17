#include "task.h"
#include "loop.h"
#include "ibus.h"
#include "config.h"
#include "upper_computer.h"
#include <string.h>
//#include "usbd_cdc_if.h"

extern uint8_t ibus_buff[IBUS_BUFF_SIZE]; 
extern uint8_t upper_receive_buff[UPPER_RECEIVE_BUFF_SIZE];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == MAIN_LOOP_TIMER_MARK)
    {
        Main_Loop();
    }
    // if(htim->Instance == SENSOR_LOOP_TIMER_MARK)
    // {
    //     Sensor_Loop();
    // }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GYRO_EXIT_Pin)
    {
        Sensor_Loop();
    }

}

void USER_IRQHandler(UART_HandleTypeDef *huart)
{
    if(huart->Instance == REMOTE_UART_MARK)                                       //判断是否是串口2
    {
        if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))         //判断是否是空闲中断
        {
            __HAL_UART_CLEAR_IDLEFLAG(huart);                           //清楚空闲中断标志（否则会一直不断进入中断）
            IBUS_UART_IDLECallback(huart);                              //调用中断处理函数
        }
    }
    if(huart->Instance == UPPER_COMPUTER_UART_MARK)                                       //判断是否是串口3
    {
        if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))         //判断是否是空闲中断
        {
            __HAL_UART_CLEAR_IDLEFLAG(huart);                           //清楚空闲中断标志（否则会一直不断进入中断）
            Upper_Computer_UART_IDLECallback(huart);                              //调用中断处理函数
        }
    }
}

// void IBUS_IRQHandler(UART_HandleTypeDef *huart)
// {
//     if(huart->Instance == USART2)                                       //判断是否是串口2
//     {
//         if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))         //判断是否是空闲中断
//         {
//             __HAL_UART_CLEAR_IDLEFLAG(huart);                           //清楚空闲中断标志（否则会一直不断进入中断）
//             IBUS_UART_IDLECallback(huart);                              //调用中断处理函数
//         }
//     }
// }

// void Upper_Computer_IRQHandler(UART_HandleTypeDef *huart)
// {
//     if(huart->Instance == USART3)                                       //判断是否是串口3
//     {
//         if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))         //判断是否是空闲中断
//         {
//             __HAL_UART_CLEAR_IDLEFLAG(huart);                           //清楚空闲中断标志（否则会一直不断进入中断）
//            Upper_Computer_UART_IDLECallback(huart);                              //调用中断处理函数
//         }
//     }
// }

void IBUS_UART_IDLECallback(UART_HandleTypeDef *huart)
{
    HAL_UART_DMAStop(huart);                                            //停止本次DMA传输
    //CDC_Transmit_FS(sbus_buff, SBUS_BUFF_SIZE);                         //测试函数：将接收到的数据打印出去
    Ibus_Parse(ibus_buff);
    memset(ibus_buff, 0, IBUS_BUFF_SIZE);                               //清零接收缓冲区
    HAL_UART_Receive_DMA(huart, (uint8_t*)ibus_buff, IBUS_BUFF_SIZE);   //重启开始DMA传输
}

void Upper_Computer_UART_IDLECallback(UART_HandleTypeDef *huart)
{
    HAL_UART_DMAStop(huart);                                            //停止本次DMA传输
    //CDC_Transmit_FS(sbus_buff, SBUS_BUFF_SIZE);                         //测试函数：将接收到的数据打印出去
    Upper_Computer_Receive_Parse(upper_receive_buff);
    memset(upper_receive_buff, 0, UPPER_RECEIVE_BUFF_SIZE);                               //清零接收缓冲区
    HAL_UART_Receive_DMA(huart, (uint8_t*)upper_receive_buff, UPPER_RECEIVE_BUFF_SIZE);   //重启开始DMA传输
}