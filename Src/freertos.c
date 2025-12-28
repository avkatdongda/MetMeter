/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTaskSampleHandle;
osThreadId myTaskUiHandle;
osThreadId myTaskMainHandle;
osThreadId myTaskBleHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTaskSample(void const * argument);
void StartTaskUi(void const * argument);
void StartTaskMain(void const * argument);
void StartTaskBle(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTaskSample */
  osThreadDef(myTaskSample, StartTaskSample, osPriorityNormal, 0, 128);
  myTaskSampleHandle = osThreadCreate(osThread(myTaskSample), NULL);

  /* definition and creation of myTaskUi */
  osThreadDef(myTaskUi, StartTaskUi, osPriorityNormal, 0, 512);
  myTaskUiHandle = osThreadCreate(osThread(myTaskUi), NULL);

  /* definition and creation of myTaskMain */
  osThreadDef(myTaskMain, StartTaskMain, osPriorityNormal, 0, 512);
  myTaskMainHandle = osThreadCreate(osThread(myTaskMain), NULL);

  /* definition and creation of myTaskBle */
  osThreadDef(myTaskBle, StartTaskBle, osPriorityNormal, 0, 128);
  myTaskBleHandle = osThreadCreate(osThread(myTaskBle), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTaskSample */
/**
* @brief Function implementing the myTaskSample thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskSample */
__weak void StartTaskSample(void const * argument)
{
  /* USER CODE BEGIN StartTaskSample */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskSample */
}

/* USER CODE BEGIN Header_StartTaskUi */
/**
* @brief Function implementing the myTaskUi thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskUi */
__weak void StartTaskUi(void const * argument)
{
  /* USER CODE BEGIN StartTaskUi */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskUi */
}

/* USER CODE BEGIN Header_StartTaskMain */
/**
* @brief Function implementing the myTaskMain thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskMain */
__weak void StartTaskMain(void const * argument)
{
  /* USER CODE BEGIN StartTaskMain */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskMain */
}

/* USER CODE BEGIN Header_StartTaskBle */
/**
* @brief Function implementing the myTaskBle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskBle */
__weak void StartTaskBle(void const * argument)
{
  /* USER CODE BEGIN StartTaskBle */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskBle */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

