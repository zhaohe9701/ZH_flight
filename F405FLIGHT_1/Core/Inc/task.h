#ifndef __TASK_H__
#define __TASK_H__
#include "flight.h"
#include "main.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

void USER_IRQHandler(UART_HandleTypeDef *huart);

//void IBUS_IRQHandler(UART_HandleTypeDef *huart);
void IBUS_UART_IDLECallback(UART_HandleTypeDef *huart);

//void Upper_Computer_IRQHandler(UART_HandleTypeDef *huart);
void Upper_Computer_UART_IDLECallback(UART_HandleTypeDef *huart);
#endif