/**
 * @file Logging.h
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief This library allows to send messages to the SWV Console.
 * In order to enable SWV feature, take a look to:
 * http://blog.atollic.com/cortex-m-debugging-printf-redirection-to-a-debugger-console-using-swv/itm-part-1
 * @version 0.1
 * @date 2019-12-27
 *
 * @copyright Copyright (c) 2019
 *
 */
// Standard libraries
#ifndef LOGGING_FEATURE_H
#define LOGGING_FEATURE_H

void LOG_INFO(const char *message);

void LOG_WARN(const char *message);

void LOG_ERROR(const char *message);

#endif  //  !LOGGING_H