#ifndef COMMON_HEADER_RTOS_UTIL
#define COMMON_HEADER_RTOS_UTIL
#include "rtx_os.h"

static inline osMutexId_t newMutexOrDie(){
	osMutexId_t mut_id = osMutexNew(NULL);
	if(mut_id == NULL)while(1);
	return mut_id;
}
static inline osSemaphoreId_t newSemaphoreOrDie(uint32_t max, uint32_t count){
	osMutexId_t sem_id = osSemaphoreNew(max, count, NULL);
	if(sem_id == NULL)while(1);
	return sem_id;
}
#endif
