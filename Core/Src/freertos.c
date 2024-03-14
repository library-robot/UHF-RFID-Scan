/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "uart_receive_dma.h"
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define transmitSignal 0x0001
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern queue8_t uart_queue;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId rfidExecuteHandle;
osThreadId transmitTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartRfidExecute(void const * argument);
void StartTransmitTask(void const * argument);

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

  /* definition and creation of rfidExecute */
  osThreadDef(rfidExecute, StartRfidExecute, osPriorityHigh, 0, 128);
  rfidExecuteHandle = osThreadCreate(osThread(rfidExecute), NULL);

  /* definition and creation of transmitTask */
  osThreadDef(transmitTask, StartTransmitTask, osPriorityIdle, 0, 128);
  transmitTaskHandle = osThreadCreate(osThread(transmitTask), NULL);

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
	  read_rfid_number();
	  osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartRfidExecute */
/**
* @brief Function implementing the rfidExecute thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRfidExecute */
void StartRfidExecute(void const * argument)
{
  /* USER CODE BEGIN StartRfidExecute */
  uint8_t read_tag_single_time[] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};

  /* Infinite loop */
  for(;;)
  {
	HAL_UART_Transmit(&huart1, read_tag_single_time, sizeof(read_tag_single_time), HAL_MAX_DELAY);
    osDelay(1000);
  }
  /* USER CODE END StartRfidExecute */
}

/* USER CODE BEGIN Header_StartTransmitTask */
/**
* @brief Function implementing the transmitTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTransmitTask */
void StartTransmitTask(void const * argument)
{
  /* USER CODE BEGIN StartTransmitTask */
  /* Infinite loop */
  osEvent event;
  for(;;)
  {

	event = osSignalWait(transmitSignal, 100);
	if(event.value.signals == transmitSignal){
		transmitData();
		}
	}
    osDelay(1);
 }
  /* USER CODE END StartTransmitTask */


/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

