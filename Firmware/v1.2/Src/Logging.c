/**
 * @file Logging.c
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief This library allows to send messages to the SWV Console.
 * @version 0.1
 * @date 2019-12-27
 *
 * @copyright Copyright (c) 2019
 *
 * Suggested configuration
 * 
 * "swoConfig": {
 *       "enabled": true,
 *       "cpuFrequency": 72000000,
 *       "swoFrequency":  2000000,
 *       "source": "probe",
 *      "decoders": [
 *         {
 *           "port": 0,
 *           "label": "Output",
 *           "type": "console"
 *         }
 *       ]
 *     },
 */

// CubeMx functions
#include "main.h"

// Noah Libraries.
#include "Logging.h"

int itm_printf(char *ptr, int len)
{
    /* Implement your write code here, this is used by puts and printf for example */
    int i = 0;
    for (i = 0; i < len; i++)
        ITM_SendChar((*ptr++));
    return len;
}

void LOG_INIT()
{
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk))
    {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
// Requeriment for STM32F7
#ifdef __STM32F7xx_HAL_H
        DWT->LAR = 0xC5ACCE55;
#endif
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }
}

void LOG_DEBUG(const char *message)
{
    char buffer[strlen(message) + 9];
    strcpy(buffer, "DEBUG: ");
    strncat(buffer, message, strlen(message) + 1);
    strncat(buffer, "\n", 2);
    itm_printf(buffer, strlen(buffer) + 1);
}

void LOG_INFO(const char *message)
{
    char buffer[strlen(message) + 8];
    strcpy(buffer, "INFO: ");
    strncat(buffer, message, strlen(message) + 1);
    strncat(buffer, "\n", 2);
    itm_printf(buffer, strlen(buffer) + 1);
}

void LOG_WARN(const char *message)
{
    char buffer[strlen(message) + 11];
    strcpy(buffer, "WARNING: ");
    strncat(buffer, message, strlen(message) + 1);
    strncat(buffer, "\n", 2);
    itm_printf(buffer, strlen(buffer) + 1);
}

void LOG_ERROR(const char *message)
{
    char buffer[strlen(message) + 9];
    strcpy(buffer, "ERROR: ");
    strncat(buffer, message, strlen(message) + 1);
    strncat(buffer, "\n", 2);
    itm_printf(buffer, strlen(buffer) + 1);
}