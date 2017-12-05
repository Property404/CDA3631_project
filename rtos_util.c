#include "rtos_util.h"

// Return a new mutex, or die trying
osMutexId_t newMutexOrDie(){
	const osMutexId_t mut_id = osMutexNew(NULL);
//	if(mut_id == NULL)while(1);
	return mut_id;
}
// Return a new semaphore, specifying the max and current count, or die trying
osSemaphoreId_t newSemaphoreOrDie(const uint32_t max, const uint32_t count){
	const osSemaphoreId_t sem_id = osSemaphoreNew(max, count, NULL);
//	if(sem_id == NULL)while(1);
	return sem_id;
}
// Etc
osMemoryPoolId_t newMemoryPoolOrDie(const uint32_t count,const uint32_t element_size){
	const osMemoryPoolId_t mp_id = osMemoryPoolNew(count, element_size, NULL);
//	if(mp_id == NULL)while(1);
	return mp_id;
}
osMessageQueueId_t newMessageQueueOrDie(const uint32_t count, const uint32_t element_size){
	const osMessageQueueId_t mq_id = osMessageQueueNew(count, element_size, NULL);
//	if(mq_id == NULL)while(1);
	return mq_id;
}
