#include "clock.h"
#include "rtos_util.h"

// Declare RTOS objects
uint32_t hour=23, minute=59, second=57;
osMutexId_t mutHour, mutMinute, mutSecond;
osSemaphoreId_t semIncMinutes, semIncHours;

// Initialization methods
void initializeClockThreads(void){
	Init_thdDisplayClock();
	Init_thdIncSeconds();
	Init_thdIncMinutes();
	Init_thdIncHours();
}

void initializeClockObjects(void){
	mutHour = newMutexOrDie();
	mutMinute = newMutexOrDie();
	mutSecond = newMutexOrDie();
	// Minutes and hours have a maximum of two
	// Because thdIncMinutes and thdIncHours are unblocked
	// In thdIncSeconds/thdIncMinutes as well as IQRs
	semIncMinutes = newSemaphoreOrDie(2, 0);
	semIncHours = newSemaphoreOrDie(2, 0);
}
