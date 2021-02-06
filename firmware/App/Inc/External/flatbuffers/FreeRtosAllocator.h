/**
 * @file FreeRtosAllocator
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Custom allocator to manage memory in flatbuffers under FreeRtos
 * @version 0.1
 * @date 01-07-2020
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

// Include
#include "cmsis_os.h"
#include "flatbuffers.h"


namespace flatbuffers {

class FreeRtosAllocator : public Allocator {
 public:
  FreeRtosAllocator() = default;
  ~FreeRtosAllocator() = default;

  // Allocate `size` bytes of memory.
  uint8_t *allocate(size_t size) override {
    return (uint8_t *) pvPortMalloc(size);
  };

  // Deallocate `size` bytes of memory at `p` allocated by this allocator.
  void deallocate(uint8_t *p, size_t)  override {
    vPortFree((void *)p);
  };

};

}
