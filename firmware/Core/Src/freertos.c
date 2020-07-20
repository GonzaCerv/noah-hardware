/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stdbool.h"
#include "NodeEntrypoint.h"
#include "NoahInfo.h"
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
NoahInfoHandler noah_info_handler;
bool initialized;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId PID_LHandle;
osThreadId PID_RHandle;
osThreadId ROSHandle;
osMessageQId msg_q_encoder_lHandle;
osMessageQId msg_q_encoder_rHandle;
osMutexId sysInfoHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void tsk_pid_l(void const * argument);
void tsk_pid_r(void const * argument);
void tsk_ros(void const * argument);

extern void MX_USB_DEVICE_Init(void);
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
  initialized = false;
  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* definition and creation of sysInfo */
  osMutexDef(sysInfo);
  sysInfoHandle = osMutexCreate(osMutex(sysInfo));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of msg_q_encoder_l */
  osMessageQDef(msg_q_encoder_l, 10, int16_t);
  msg_q_encoder_lHandle = osMessageCreate(osMessageQ(msg_q_encoder_l), NULL);

  /* definition and creation of msg_q_encoder_r */
  osMessageQDef(msg_q_encoder_r, 10, int16_t);
  msg_q_encoder_rHandle = osMessageCreate(osMessageQ(msg_q_encoder_r), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of PID_L */
  osThreadDef(PID_L, tsk_pid_l, osPriorityNormal, 0, 400);
  PID_LHandle = osThreadCreate(osThread(PID_L), NULL);

  /* definition and creation of PID_R */
  osThreadDef(PID_R, tsk_pid_r, osPriorityNormal, 0, 400);
  PID_RHandle = osThreadCreate(osThread(PID_R), NULL);

  /* definition and creation of ROS */
  osThreadDef(ROS, tsk_ros, osPriorityNormal, 0, 2500);
  ROSHandle = osThreadCreate(osThread(ROS), NULL);

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
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  noahInfo_init(&sysInfoHandle, &noah_info_handler);
  initialized = true;
  PowerManagementNode();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_tsk_pid_l */
/**
* @brief Function implementing the PID_L thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_tsk_pid_l */
void tsk_pid_l(void const * argument)
{
  /* USER CODE BEGIN tsk_pid_l */
  while(!initialized){
    osDelay(2);
  }
  PIDLNode(&noah_info_handler);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END tsk_pid_l */
}

/* USER CODE BEGIN Header_tsk_pid_r */
/**
* @brief Function implementing the PID_R thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_tsk_pid_r */
void tsk_pid_r(void const * argument)
{
  /* USER CODE BEGIN tsk_pid_r */
  while(!initialized){
    osDelay(2);
  }
  PIDRNode(&noah_info_handler);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END tsk_pid_r */
}

/* USER CODE BEGIN Header_tsk_ros */
/**
* @brief Function implementing the ROS thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_tsk_ros */
void tsk_ros(void const * argument)
{
  /* USER CODE BEGIN tsk_ros */
  while(!initialized){
    osDelay(2);
  }
  ROSCommsNode(&noah_info_handler);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END tsk_ros */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
