/* This header contains general helper functions not specific
 * to any component of the project */
#ifndef COMMON_HEADER_RTOS_UTIL
#define COMMON_HEADER_RTOS_UTIL
#include "rtx_os.h"

// Return a new mutex, or die trying
static inline osMutexId_t newMutexOrDie(){
	osMutexId_t mut_id = osMutexNew(NULL);
	if(mut_id == NULL)while(1);
	return mut_id;
}
// Return a new semaphore, specifying the max and current count, or die trying
static inline osSemaphoreId_t newSemaphoreOrDie(uint32_t max, uint32_t count){
	osSemaphoreId_t sem_id = osSemaphoreNew(max, count, NULL);
	if(sem_id == NULL)while(1);
	return sem_id;
}
// Etc
static inline osMemoryPoolId_t newMemoryPoolOrDie(uint32_t count, uint32_t element_size){
	osMemoryPoolId_t mp_id = osMemoryPoolNew(count, element_size, NULL);
	if(mp_id == NULL)while(1);
	return mp_id;
}
static inline osMessageQueueId_t newMessageQueueOrDie(uint32_t count, uint32_t element_size){
	osMessageQueueId_t mq_id = osMessageQueueNew(count, element_size, NULL);
	if(mq_id == NULL)while(1);
	return mq_id;
}
#endif
