/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_ONBOARD_Pin GPIO_PIN_13
#define LED_ONBOARD_GPIO_Port GPIOC
#define LED2_1_Pin GPIO_PIN_1
#define LED2_1_GPIO_Port GPIOA
#define LED2_2_Pin GPIO_PIN_2
#define LED2_2_GPIO_Port GPIOA
#define LED2_3_Pin GPIO_PIN_3
#define LED2_3_GPIO_Port GPIOA
#define BT_ENABLE_Pin GPIO_PIN_0
#define BT_ENABLE_GPIO_Port GPIOB
#define BT_STATE_Pin GPIO_PIN_1
#define BT_STATE_GPIO_Port GPIOB
#define BT_TX_Pin GPIO_PIN_10
#define BT_TX_GPIO_Port GPIOB
#define BT_RX_Pin GPIO_PIN_11
#define BT_RX_GPIO_Port GPIOB
#define LCD_ENABLE_Pin GPIO_PIN_12
#define LCD_ENABLE_GPIO_Port GPIOB
#define LED1_3_Pin GPIO_PIN_8
#define LED1_3_GPIO_Port GPIOA
#define LED1_2_Pin GPIO_PIN_9
#define LED1_2_GPIO_Port GPIOA
#define LED1_1_Pin GPIO_PIN_10
#define LED1_1_GPIO_Port GPIOA
#define BUTTON4_Pin GPIO_PIN_11
#define BUTTON4_GPIO_Port GPIOA
#define BUTTON3_Pin GPIO_PIN_15
#define BUTTON3_GPIO_Port GPIOA
#define BUTTON2_Pin GPIO_PIN_3
#define BUTTON2_GPIO_Port GPIOB
#define BUTTON1_Pin GPIO_PIN_4
#define BUTTON1_GPIO_Port GPIOB
#define PIR_Signal_Pin GPIO_PIN_7
#define PIR_Signal_GPIO_Port GPIOB
#define LCD_SCL_Pin GPIO_PIN_8
#define LCD_SCL_GPIO_Port GPIOB
#define LCD_SDA_Pin GPIO_PIN_9
#define LCD_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
