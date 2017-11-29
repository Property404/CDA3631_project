#include "clock.h"
#include "rtos_util.h"

// RTOS objects
uint32_t hour=23, minute=59, second=57;
osMutexId_t mutHour, mutMinute, mutSecond;
osSemaphoreId_t semIncMinutes, semIncHours;

// Initialization methods
// We define them here to make the
// main.c file cleaner
void initializeClockThreads(void){
	if(-1 == Init_thdDisplayClock())while(1);
	if(-1 == Init_thdIncSeconds())while(1);
	if(-1 == Init_thdIncMinutes())while(1);
	if(-1 == Init_thdIncHours())while(1);
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
