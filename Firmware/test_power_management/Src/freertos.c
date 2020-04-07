/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

// Standard libraries
#include "stdbool.h"

// CMSIS Libraries

// Noone libraries
#include "Battery.h"
#include "Button.h"
#include "Config.h"
#include "PowerManagement.h"

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
float      motorKP, motorKI, motorKD;
float      targetSpeedL, targetSpeedR;
osThreadId thread_id;
/* USER CODE END Variables */
osThreadId StartTaskHandle;
osThreadId PowerManagementHandle;
osThreadId ButtonHandle;
osThreadId BatteryHandle;
osThreadId MotionControlHandle;
osThreadId UARTCommsHandle;
osThreadId IRFloorHandle;
osMutexId led_mutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* USER CODE END FunctionPrototypes */

void start_tsk(void const * argument);
void PowerMgmt_tsk(void const * argument);
void Button_tsk(void const * argument);
void Battery_tsk(void const * argument);
void Motion_tsk(void const * argument);
void Uart_tsk(void const * argument);
void Ir_floor_tsk(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t  xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize) {
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

  /* Create the mutex(es) */
  /* definition and creation of led_mutex */
  osMutexDef(led_mutex);
  led_mutexHandle = osMutexCreate(osMutex(led_mutex));

  /* USER CODE BEGIN RTOS_MUTEX */
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
  /* definition and creation of StartTask */
  osThreadDef(StartTask, start_tsk, osPriorityNormal, 0, 128);
  StartTaskHandle = osThreadCreate(osThread(StartTask), NULL);

  /* definition and creation of PowerManagement */
  osThreadDef(PowerManagement, PowerMgmt_tsk, osPriorityBelowNormal, 0, 512);
  PowerManagementHandle = osThreadCreate(osThread(PowerManagement), NULL);

  /* definition and creation of Button */
  osThreadDef(Button, Button_tsk, osPriorityNormal, 0, 256);
  ButtonHandle = osThreadCreate(osThread(Button), NULL);

  /* definition and creation of Battery */
  osThreadDef(Battery, Battery_tsk, osPriorityLow, 0, 128);
  BatteryHandle = osThreadCreate(osThread(Battery), NULL);

  /* definition and creation of MotionControl */
  osThreadDef(MotionControl, Motion_tsk, osPriorityNormal, 0, 256);
  MotionControlHandle = osThreadCreate(osThread(MotionControl), NULL);

  /* definition and creation of UARTComms */
  osThreadDef(UARTComms, Uart_tsk, osPriorityNormal, 0, 256);
  UARTCommsHandle = osThreadCreate(osThread(UARTComms), NULL);

  /* definition and creation of IRFloor */
  osThreadDef(IRFloor, Ir_floor_tsk, osPriorityNormal, 0, 256);
  IRFloorHandle = osThreadCreate(osThread(IRFloor), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_start_tsk */
/**
 * @brief  Function implementing the StartTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_start_tsk */
void start_tsk(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN start_tsk */
    HAL_GPIO_WritePin(EN_3V_GPIO_Port, EN_3V_Pin, GPIO_PIN_SET);
    /* Infinite loop */
    for (;;) {
        osDelay(1000);
    }
  /* USER CODE END start_tsk */
}

/* USER CODE BEGIN Header_PowerMgmt_tsk */
/**
 * @brief Function implementing the PowerManagement thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_PowerMgmt_tsk */
void PowerMgmt_tsk(void const * argument)
{
  /* USER CODE BEGIN PowerMgmt_tsk */
    bool pwrChanged = false;
    pwr_init();
    osSignalSet(UARTCommsHandle, POWER_MGMT_SIGNAL_READY);
    HAL_GPIO_WritePin(BQ24070_MODE_PORT,BQ24070_MODE_PIN,GPIO_PIN_SET);
    /* Infinite loop */
    for (;;) {
      //If changes were alerted before but still were not be able to show that with the leds,
      //Check if there is a power update but do not change the flag.
      if (pwrChanged)
      {
        //Try to consume the mutex.
        if (osMutexWait(led_mutexHandle, LED_MUTEX_WAIT_TIME_MS) == osOK)
        {
          switch (pwr_get_current_state())
          {
          case DISCHARGING:
            HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_PIN, GPIO_PIN_SET);
            break;
          case CHARGING_USB:
          case CHARGING_HIGH_PWR:
            HAL_GPIO_TogglePin(RED_LED_PORT,RED_LED_PIN);
            HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_PIN, GPIO_PIN_RESET);
            break;
          default:
            HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_PIN, GPIO_PIN_RESET);
            break;
          }
          //Change the flag, allowing to know that the leds were updated accordingly to the status..
          pwrChanged = false;
          //Free the mutex.
          osMutexRelease(led_mutexHandle);
        }
      }
      else
      {
        pwrChanged = pwr_update();
      }
      osDelay(POWER_MGMT_UPDATE_RATE_MS);
    }
  /* USER CODE END PowerMgmt_tsk */
}

/* USER CODE BEGIN Header_Button_tsk */
/**
 * @brief Function implementing the Button thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Button_tsk */
void Button_tsk(void const * argument)
{
  /* USER CODE BEGIN Button_tsk */
    osSignalSet(UARTCommsHandle, BUTTON_SIGNAL_READY);
    /* Infinite loop */
    for (;;) {
        button_update();
        osDelay(BUTTON_UPDATE_RATE_MS);
    }
  /* USER CODE END Button_tsk */
}

/* USER CODE BEGIN Header_Battery_tsk */
/**
 * @brief Function implementing the Battery thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Battery_tsk */
void Battery_tsk(void const * argument)
{
  /* USER CODE BEGIN Battery_tsk */
    bat_init();
    osSignalSet(UARTCommsHandle, BATTERY_SIGNAL_READY);
    /* Infinite loop */
    for (;;) {
        bat_update();
        osDelay(BAT_CHECKING_UPDATE_RATE_MS);
    }
  /* USER CODE END Battery_tsk */
}

/* USER CODE BEGIN Header_Motion_tsk */
/**
 * @brief Function implementing the MotionControl thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Motion_tsk */
void Motion_tsk(void const * argument)
{
  /* USER CODE BEGIN Motion_tsk */
    /* Infinite loop */
    for (;;) {
        osDelay(MOTION_UPDATE_RATE);
    }
  /* USER CODE END Motion_tsk */
}

/* USER CODE BEGIN Header_Uart_tsk */
/**
 * @brief Function implementing the UARTComms thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Uart_tsk */
void Uart_tsk(void const * argument)
{
  /* USER CODE BEGIN Uart_tsk */
    /* Infinite loop */
    for (;;) {
        osDelay(UART_COMMS_UPDATE_RATE);
    }
  /* USER CODE END Uart_tsk */
}

/* USER CODE BEGIN Header_Ir_floor_tsk */
/**
 * @brief Function implementing the IRFloor thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Ir_floor_tsk */
void Ir_floor_tsk(void const * argument)
{
  /* USER CODE BEGIN Ir_floor_tsk */
    /* Infinite loop */
    for (;;) {
        osDelay(FLOOR_IR_UPDATE_RATE);
    }
  /* USER CODE END Ir_floor_tsk */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
