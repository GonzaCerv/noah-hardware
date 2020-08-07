/**
 * @file PowerManagement.cpp
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Power management class for Noah robot
 * @version 0.1
 * @date 01-07-2020
 *
 * @copyright Copyright (c) 2020
 *
 */

// Standard libraries
#include "stdlib.h"

// CubeMx libraries
#include "usart.h"

// Noah libraries
#include "flatbuffers/DataPackage_generated.h"
#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/FreeRtosAllocator.h"
#include "ROScomms/ROSComms.hpp"

extern float target_speed_;
extern float current_speed_;

namespace noah {
namespace ros {

ROSComms::ROSComms(const UART_HandleTypeDef &ros_port, const float update_rate, NoahInfoHandler *noah_info_handler) :
    ros_port_ { ros_port },
    update_rate_ { update_rate },
    noah_info_handler_ { noah_info_handler } {
  // Enable reception with DMA and enable USART idle interruption
  HAL_UART_Receive_DMA(&ros_port_, buffer_, UART_BUFFER_SIZE);
  enableUsartIdleIT();
}

int ROSComms::run() {
  while (1) {
    if (isNewDataAvailable()) {
      processIncommingPackage();
    }
    osDelay(update_rate_);
  }
  return EXIT_SUCCESS;
}

void ROSComms::processIncommingPackage() {
  bool send_response = false;
  // Stop reception to process the new packages.
  HAL_UART_DMAStop(&ros_port_);

  // Objects that allows to serialize the response.
  flatbuffers::FreeRtosAllocator allocator;
  flatbuffers::FlatBufferBuilder builder(100, &allocator);
  DataPackageBuilder response_builder(builder);

  // Deserialize the incoming package.
  auto incomming_package = GetDataPackage(&buffer_[0]);

  // Check if there is an encoderRequest

  if (flatbuffers::IsFieldPresent(incomming_package, DataPackage::VT_ENCODERREQUEST)) {
    auto ticks_l = noahInfo_get_ticks_l(noah_info_handler_);
    // Invert speed to make to robot go forward.
    ticks_l = -ticks_l;
    auto ticks_r = noahInfo_get_ticks_r(noah_info_handler_);

    auto encoder_response = EncoderResponse(ticks_l, ticks_r);
    response_builder.add_encoderResponse(&encoder_response);
    send_response = true;
  }

  // Check if there is a new target speed for motor L
  if (flatbuffers::IsFieldPresent(incomming_package, DataPackage::VT_TARGETSPEEDLREQUEST)) {
    // Invert speed to make to robot go forward.
    noahInfo_set_target_speed_l(noah_info_handler_, -incomming_package->targetSpeedLRequest());
  }

  // Check if there is a new target speed for motor R
  if (flatbuffers::IsFieldPresent(incomming_package, DataPackage::VT_TARGETSPEEDRREQUEST)) {
    noahInfo_set_target_speed_r(noah_info_handler_, incomming_package->targetSpeedRRequest());
  }

  if (send_response) {
    // Finish the buffer and serialize
    auto response = response_builder.Finish();
    builder.Finish(response);
    uint8_t *response_buffer = builder.GetBufferPointer();

    // Transmit
    uint16_t size = builder.GetSize();
    HAL_UART_Transmit(&ros_port_, response_buffer, size, 10);
  }
  // Enable the DMA reception again and clear the flag.
  HAL_UART_Receive_DMA(&ros_port_, buffer_, UART_BUFFER_SIZE);
  clearFlagUsartIdleIT();
}

void ROSComms::enableUsartIdleIT() {
  SET_BIT(ros_port_.Instance->CR1, USART_CR1_IDLEIE);
}

bool ROSComms::isNewDataAvailable() {
  if (READ_BIT(ros_port_.Instance->SR, USART_SR_IDLE) == (USART_SR_IDLE)) {
    return true;
  } else {
    return false;
  }
}

void ROSComms::clearFlagUsartIdleIT() {
  __IO uint32_t tmpreg;
  tmpreg = ros_port_.Instance->SR;
  (void) tmpreg;
  tmpreg = ros_port_.Instance->DR;
  (void) tmpreg;
}

}
}
