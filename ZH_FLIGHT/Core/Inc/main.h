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
#include "stm32h7xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void UsbPrintf(const char *format, ...);
void test();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_3
#define LED_GPIO_Port GPIOC
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define SPI1_EXIT_Pin GPIO_PIN_4
#define SPI1_EXIT_GPIO_Port GPIOC
#define SPI1_EXIT_EXTI_IRQn EXTI4_IRQn
#define SPI3_CS1_Pin GPIO_PIN_15
#define SPI3_CS1_GPIO_Port GPIOA
#define SPI3_CS2_Pin GPIO_PIN_0
#define SPI3_CS2_GPIO_Port GPIOD
#define SPI3_EXIT1_Pin GPIO_PIN_1
#define SPI3_EXIT1_GPIO_Port GPIOD
#define SPI3_EXIT1_EXTI_IRQn EXTI1_IRQn
#define SPI3_EXIT2_Pin GPIO_PIN_2
#define SPI3_EXIT2_GPIO_Port GPIOD
#define SPI3_EXIT2_EXTI_IRQn EXTI2_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
