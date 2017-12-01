/* This header contains general helper functions not specific
 * to any component of the project */
#ifndef COMMON_HEADER_RTOS_UTIL
#define COMMON_HEADER_RTOS_UTIL
#include "rtx_os.h"

// Return a new mutex, or die trying
osMutexId_t newMutexOrDie(void);

// Return a new semaphore, specifying the max and current count, or die trying
osSemaphoreId_t newSemaphoreOrDie(const uint32_t max, const uint32_t count);

// Etc
osMemoryPoolId_t newMemoryPoolOrDie(const uint32_t count, const uint32_t element_size);

osMessageQueueId_t newMessageQueueOrDie(const uint32_t count, const uint32_t element_size);
#endif
