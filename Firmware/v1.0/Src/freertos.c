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
#include "math.h"
#include "stdbool.h"

// CMSIS Libraries
#include "usart.h"
// Noone libraries
#include "Battery.h"
#include "Button.h"
#include "Config.h"
#include "IRFloor.h"
#include "IRMotion.h"
#include "Motor.h"
#include "PowerManagement.h"
#include "Servo.h"
#include "UsartCommunication.h"

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
float      targetSpeedL = 0.0, targetSpeedR;
float      currentSpeedL = 0.0, currentSpeedR = 0.0;
float      integrationSumL = 0.0, integrationSumR = 0.0;
float      previousErrorL = 0.0, previousErrorR = 0.0;
float      current_error_l;
float      derivative;
int16_t    duty_l, duty_r;
osThreadId thread_id;
/* USER CODE END Variables */
osThreadId StartTaskHandle;
osThreadId PowerManagementHandle;
osThreadId ButtonHandle;
osThreadId BatteryHandle;
osThreadId MotionControlHandle;
osThreadId UARTCommsHandle;
osThreadId IRFloorHandle;
osThreadId MotionIRHandle;
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
void MotionIR_tsk(void const * argument);

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
  osThreadDef(PowerManagement, PowerMgmt_tsk, osPriorityBelowNormal, 0, 400);
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
  osThreadDef(UARTComms, Uart_tsk, osPriorityNormal, 0, 512);
  UARTCommsHandle = osThreadCreate(osThread(UARTComms), NULL);

  /* definition and creation of IRFloor */
  osThreadDef(IRFloor, Ir_floor_tsk, osPriorityNormal, 0, 128);
  IRFloorHandle = osThreadCreate(osThread(IRFloor), NULL);

  /* definition and creation of MotionIR */
  osThreadDef(MotionIR, MotionIR_tsk, osPriorityNormal, 0, 256);
  MotionIRHandle = osThreadCreate(osThread(MotionIR), NULL);

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
    /* Infinite loop */
    for (;;) {
        osDelay(1);
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
    // bool pwrChanged = false;
    // pwr_init();
    osSignalSet(UARTCommsHandle, POWER_MGMT_SIGNAL_READY);
    /* Infinite loop */
    for (;;) {
        // If changes were alerted before but still were not be able to show that with the leds,
        // Check if there is a power update but do not change the flag.
        // if (pwrChanged) {
        //     pwr_update();
        //     // Try to consume the mutex.
        //     if (osMutexWait(led_mutexHandle, LED_MUTEX_WAIT_TIME_MS) == osOK) {
        //         switch (pwr_get_current_state()) {
        //             case CHARGING_USB:
        //                 HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_PIN, GPIO_PIN_SET);
        //                 HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_PIN, GPIO_PIN_RESET);
        //                 break;
        //             case CHARGING_HIGH_PWR:
        //                 HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_PIN, GPIO_PIN_SET);
        //                 HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_PIN, GPIO_PIN_SET);
        //                 break;
        //             default:
        //                 HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_PIN, GPIO_PIN_RESET);
        //                 HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_PIN, GPIO_PIN_RESET);
        //                 break;
        //         }
        //         // Change the flag, allowing to know that the leds were updated accordingly to the status..
        //         pwrChanged = false;
        //         // Free the mutex.
        //         osMutexRelease(led_mutexHandle);
        //     }
        // } else {
        //     pwrChanged = pwr_update();
        // }
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
        // button_update();
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
    motorKP = KP_VAL;
    motorKI = KI_VAL;
    motorKD = KD_VAL;
    motor_init(&htim2, TIM_CHANNEL_2, TIM_CHANNEL_3);
    osSignalSet(UARTCommsHandle, MOTION_CONTROL_SIGNAL_READY);
    osSignalWait(IR_SENSOR_SIGNAL_READY, osWaitForever);
    /* Infinite loop */
    for (;;) {
        osDelay(PID_UPDATE_RATE_MS);
        // Get the current speed of each motor.
        currentSpeedL = irmotion_get_speed(MOTOR_L);
        currentSpeedR = irmotion_get_speed(MOTOR_R);

        // Implementación PID for motor L.
        current_error_l = targetSpeedL - currentSpeedL;
        // We divide by 1000 to convert miliseconds in seconds.
        integrationSumL += (current_error_l * (PID_UPDATE_RATE_MS / 1000.0));
        derivative = (current_error_l - previousErrorL) / (PID_UPDATE_RATE_MS / 1000.0);
        duty_l = (int16_t)((motorKP * current_error_l) + (motorKI * integrationSumL) + (motorKD * derivative));
        previousErrorL = current_error_l;
        if (targetSpeedL > 0.0) {
            if (duty_l > MAX_DUTY_PWM)
                duty_l = MAX_DUTY_PWM;
            else if (duty_l < 0)
                duty_l = 0;
        } else {
            if (duty_l < (-MAX_DUTY_PWM))
                duty_l = -MAX_DUTY_PWM;
            else if (duty_l > 0)
                duty_l = 0;
        }

        motor_set_duty(MOTOR_L, duty_l);

        // Implementación PID for motor R.
        // float currentError_r = targetSpeedR - currentSpeedR;
        // // We divide by 1000 to convert miliseconds in seconds.
        // integrationSumR += currentError_r * (PID_UPDATE_RATE_MS / 1000.0);
        // duty_r = (int16_t)(motorKP * currentError_r + motorKI * integrationSumR +
        //                 motorKD * (currentError_r - previousErrorR) / (PID_UPDATE_RATE_MS / 1000.0));
        // previousErrorR = currentError_r;
        // if (duty_r > MAX_DUTY_PWM) duty_r = MAX_DUTY_PWM;
        // // duty_l = duty_l * (MAX_DUTY_PWM/32767);
        // if(duty_l > 0){
        //   if (duty_l > MAX_DUTY_PWM) duty_l = MAX_DUTY_PWM;
        // }
        // else{
        //   if (abs(duty_l) > MAX_DUTY_PWM) duty_l = -MAX_DUTY_PWM;
        // }
        // motor_set_duty(MOTOR_R, duty_r);
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
    // Wait for all the signals in order to start UART communications
    osSignalWait(BUTTON_SIGNAL_READY, osWaitForever);
    osSignalWait(BATTERY_SIGNAL_READY, osWaitForever);
    osSignalWait(MOTION_CONTROL_SIGNAL_READY, osWaitForever);
    // osSignalWait(START_TASK_SIGNAL_READY, osWaitForever);
    // osSignalWait(SERVO_SIGNAL_READY, osWaitForever);
    // osSignalWait(POWER_MGMT_SIGNAL_READY, osWaitForever);
    usartComms_init(&huart1);

    /* Infinite loop */
    for (;;) {
        usartComms_update();
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
    bool ir_front_detecting = true;
    bool ir_back_detecting = true;
    /* Infinite loop */
    for (;;) {
        if (irfloor_is_back_detecting() != ir_front_detecting) {
            ir_back_detecting = irfloor_is_back_detecting();
            irfloor_advertise_Change(BACK_IR, ir_back_detecting);
        }
        if (irfloor_is_front_detecting() != ir_front_detecting) {
            ir_front_detecting = irfloor_is_front_detecting();
            irfloor_advertise_Change(FRONT_IR, ir_front_detecting);
        }
        osDelay(FLOOR_IR_UPDATE_RATE);
    }
  /* USER CODE END Ir_floor_tsk */
}

/* USER CODE BEGIN Header_MotionIR_tsk */
/**
 * @brief Function implementing the MotionIR thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_MotionIR_tsk */
void MotionIR_tsk(void const * argument)
{
  /* USER CODE BEGIN MotionIR_tsk */
    irmotion_init(&htim3);
    osSignalSet(MotionControlHandle, IR_SENSOR_SIGNAL_READY);
    irmotion_start();
    /* Infinite loop */
    for (;;) {
        osDelay(100);
        irmotion_update();
        irmotion_reset();
    }
  /* USER CODE END MotionIR_tsk */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
