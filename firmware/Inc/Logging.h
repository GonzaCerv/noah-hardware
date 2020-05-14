/**
 * @file Logging.h
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief This library allows to send messages to the SWV Console.
 * In order to enable SWV feature, take a look to <a href="https://www.st.com/content/ccc/resource/technical/document/application_note/group0/3d/a5/0e/30/76/51/45/58/DM00354244/files/DM00354244.pdf/jcr:content/translations/en.DM00354244.pdf">this</a> 
 * This library was tested on VScode using OpenOCD and the Cortex-Debug plugin.
 * @version 0.1
 * @date 2019-12-27
 *
 * @copyright Copyright (c) 2019
 *
 *  The configuration needed in the json configuration file to use SWD is provided here: 
 * 
 *  "swoConfig": {
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
// Standard libraries
#ifndef LOGGING_FEATURE_H
#define LOGGING_FEATURE_H

// Standard functions
#include <stdio.h>
#include <string.h>

void LOG_INIT();

/**
 * @brief Sends an info message.
 * 
 * @param message 
 */
void LOG_DEBUG(const char *message);

/**
 * @brief Sends an info message.
 * 
 * @param message 
 */
void LOG_INFO(const char *message);

/**
 * @brief Sends an warning message.
 * 
 * @param message 
 */
void LOG_WARN(const char *message);

/**
 * @brief Sends an error message.
 * 
 * @param message 
 */
void LOG_ERROR(const char *message);

/**
 * @brief Prints messages to the ITM console
 * 
 * @param ptr String to be printed
 * @param len len of the string.
 * @return int number of elements printed.
 */
int itm_printf(char *ptr, int len);

/**
 * @brief Starts measuring the tick count.
 * 
 */
inline void LOG_START_TICK_MEASURE()
{
    DWT->CYCCNT = 0;
}

/**
 * @brief Prints in the console the amount of ticks since the last call 
 * to LOG_START_TICK_MEASURE().
 * 
 */
inline uint32_t LOG_GET_TICK_MEASURE()
{
    uint32_t count = DWT->CYCCNT;
    char buffer[50];
    sprintf(buffer, "CYCLE COUNT: %lu\n", count);
    itm_printf(buffer, strlen(buffer) + 1);
    return count;
}

#endif //  !LOGGING_H