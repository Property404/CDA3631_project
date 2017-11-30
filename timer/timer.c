#include "timer.h"
#include "rtos_util.h"

uint32_t timer_minute = 0, timer_second = 0, timer_centisecond = 0;
osMutexId_t mutTimerMinute, mutTimerSecond, mutTimerCentisecond;
osSemaphoreId_t semTimer, semIncTimerSeconds, semIncTimerMinutes;

void initializeTimerThreads(void){
	if(-1 == Init_thdDisplayTimer())while(1){}
	if(-1 == Init_thdIncTimer())while(1){}
	if(-1 == Init_thdIncTimerSeconds())while(1){}
	if(-1 == Init_thdIncTimerMinutes())while(1){}
}

void initializeTimerObjects(void){
	mutTimerCentisecond = newMutexOrDie();
	mutTimerMinute = newMutexOrDie();
	mutTimerSecond = newMutexOrDie();
	/* Unoike the clock, each of these semaphore can only be affected by
	 * one thing, so we set max count to 1 */
	semTimer = newSemaphoreOrDie(1, 0);
	semIncTimerSeconds = newSemaphoreOrDie(1, 0);
	semIncTimerMinutes = newSemaphoreOrDie(1, 0);
}
