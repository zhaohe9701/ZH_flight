/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void usb_printf(const char *format, ...);
uint8_t Usb_Receive(uint8_t *buf);
//CDC_Transmit_FS(UserTxBufferFS, length);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_15
#define LED1_GPIO_Port GPIOC
#define GYRO_CS_Pin GPIO_PIN_4
#define GYRO_CS_GPIO_Port GPIOA
#define GYRO_SCK_Pin GPIO_PIN_5
#define GYRO_SCK_GPIO_Port GPIOA
#define GYRO_MISO_Pin GPIO_PIN_6
#define GYRO_MISO_GPIO_Port GPIOA
#define GYRO_MOSI_Pin GPIO_PIN_7
#define GYRO_MOSI_GPIO_Port GPIOA
#define GYRO_EXIT_Pin GPIO_PIN_4
#define GYRO_EXIT_GPIO_Port GPIOC
#define GYRO_EXIT_EXTI_IRQn EXTI4_IRQn
#define MOTOR1_Pin GPIO_PIN_0
#define MOTOR1_GPIO_Port GPIOB
#define MOTOR2_Pin GPIO_PIN_1
#define MOTOR2_GPIO_Port GPIOB
#define MOTOR4_Pin GPIO_PIN_8
#define MOTOR4_GPIO_Port GPIOC
#define MOTOR3_Pin GPIO_PIN_9
#define MOTOR3_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
