/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CHG_IN_Pin GPIO_PIN_0
#define CHG_IN_GPIO_Port GPIOC
#define CHG_ON_Pin GPIO_PIN_1
#define CHG_ON_GPIO_Port GPIOC
#define CHG_DONE_Pin GPIO_PIN_2
#define CHG_DONE_GPIO_Port GPIOC
#define BAT_ADC_Pin GPIO_PIN_3
#define BAT_ADC_GPIO_Port GPIOC
#define PUMP_POW_Pin GPIO_PIN_1
#define PUMP_POW_GPIO_Port GPIOA
#define OXY_TXD_Pin GPIO_PIN_2
#define OXY_TXD_GPIO_Port GPIOA
#define OXY_RXD_Pin GPIO_PIN_3
#define OXY_RXD_GPIO_Port GPIOA
#define OXY_EN_Pin GPIO_PIN_4
#define OXY_EN_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_6
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_0
#define LED3_GPIO_Port GPIOB
#define SHT20_SCL_Pin GPIO_PIN_10
#define SHT20_SCL_GPIO_Port GPIOB
#define SHT20_SDA_Pin GPIO_PIN_11
#define SHT20_SDA_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOB
#define LCD_SCK_Pin GPIO_PIN_13
#define LCD_SCK_GPIO_Port GPIOB
#define LCD_DC_Pin GPIO_PIN_14
#define LCD_DC_GPIO_Port GPIOB
#define LCD_SDA_Pin GPIO_PIN_15
#define LCD_SDA_GPIO_Port GPIOB
#define LCD_BLK_Pin GPIO_PIN_6
#define LCD_BLK_GPIO_Port GPIOC
#define LCD_RST_Pin GPIO_PIN_7
#define LCD_RST_GPIO_Port GPIOC
#define KEY4_Pin GPIO_PIN_8
#define KEY4_GPIO_Port GPIOC
#define KEY3_Pin GPIO_PIN_9
#define KEY3_GPIO_Port GPIOC
#define KEY2_Pin GPIO_PIN_8
#define KEY2_GPIO_Port GPIOA
#define KEY1_Pin GPIO_PIN_9
#define KEY1_GPIO_Port GPIOA
#define BEEP_Pin GPIO_PIN_10
#define BEEP_GPIO_Port GPIOA
#define LED_ALARM_Pin GPIO_PIN_11
#define LED_ALARM_GPIO_Port GPIOA
#define BW_EN_Pin GPIO_PIN_12
#define BW_EN_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_15
#define FLASH_CS_GPIO_Port GPIOA
#define BW_TXD_Pin GPIO_PIN_10
#define BW_TXD_GPIO_Port GPIOC
#define BW_RXD_Pin GPIO_PIN_11
#define BW_RXD_GPIO_Port GPIOC
#define DBG_TXD_Pin GPIO_PIN_12
#define DBG_TXD_GPIO_Port GPIOC
#define DBG_RXD_Pin GPIO_PIN_2
#define DBG_RXD_GPIO_Port GPIOD
#define FLASH_SCK_Pin GPIO_PIN_3
#define FLASH_SCK_GPIO_Port GPIOB
#define FLASH_MISO_Pin GPIO_PIN_4
#define FLASH_MISO_GPIO_Port GPIOB
#define FLASH_MOSI_Pin GPIO_PIN_5
#define FLASH_MOSI_GPIO_Port GPIOB
#define TCD_TXD_Pin GPIO_PIN_6
#define TCD_TXD_GPIO_Port GPIOB
#define TCD_RXD_Pin GPIO_PIN_7
#define TCD_RXD_GPIO_Port GPIOB
#define WF5803_SCL_Pin GPIO_PIN_8
#define WF5803_SCL_GPIO_Port GPIOB
#define WF5803_SDA_Pin GPIO_PIN_9
#define WF5803_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
