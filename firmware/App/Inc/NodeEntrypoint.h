/**
 * @file NodeEntrypoint.h
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Provides an entrypoint for each of the C++ implementations that must run in each thread.
 * @version 0.1
 * @date 01-07-2020
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef INC_NODEENTRYPOINT_H_
#define INC_NODEENTRYPOINT_H_

#include "Utils/NoahInfo.h"

#ifdef __cplusplus
extern "C" {
#endif

int PowerManagementNode();
int PIDLNode(NoahInfoHandler *noah_info_handler);
int PIDRNode(NoahInfoHandler *noah_info_handler);
int ROSCommsNode(NoahInfoHandler *noah_info_handler);

#ifdef __cplusplus
}
#endif

#endif /* INC_NODEENTRYPOINT_H_ */
