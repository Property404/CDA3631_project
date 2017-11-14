#include "timer.h"
#include "rtos_util.h"

uint32_t timer_minute = 0, timer_second = 0, timer_centisecond = 0;
osMutexId_t mutTimerMinute, mutTimerSecond, mutTimerCentisecond;
osSemaphoreId_t semTimer, semIncTimerSeconds;

void initializeTimerThreads(void){
	Init_thdDisplayTimer();
	Init_thdIncTimer();
	Init_thdIncTimerSeconds();
}

void initializeTimerObjects(void){
	mutTimerCentisecond = newMutexOrDie();
	mutTimerMinute = newMutexOrDie();
	mutTimerSecond = newMutexOrDie();
	semTimer = newSemaphoreOrDie(1, 0);
	semIncTimerSeconds = newSemaphoreOrDie(1, 0);
}
