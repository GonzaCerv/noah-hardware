/**
 * @file Logging.h
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief This library allows to send messages to the SWV Console.
 * @version 0.1
 * @date 2019-12-27
 *
 * @copyright Copyright (c) 2019
 *
 */

// Standard functions.
#include <stdio.h>
#include <sys/unistd.h>

// CubeMx functions
#include "main.h"

// Noah Libraries.
#include "Logging.h"

/// @brief Overload function used for printf.
int _write(int file, char *ptr, int len) {
    /* Implement your write code here, this is used by puts and printf for example */
    int i = 0;
    for (i = 0; i < len; i++) ITM_SendChar((*ptr++));
    return len;
}

void LOG_INFO(const char *message) { printf("INFO: %s", message); }

void LOG_WARN(const char *message) { printf("WARNING: %s", message); }

void LOG_ERROR(const char *message) { printf("ERROR: %s", message); }