#include "cmsis_os2.h"
#include "rtx_os.h"
#include "timer.h"
 
/*Increments seconds for the timer. When seconds reaches 60, minutes is incremented */
 
void thdIncTimerSeconds (void *argument);                                 // thread function
osThreadId_t tid_thdIncTimerSeconds;                                      // thread id
 
int Init_thdIncTimerSeconds (void) {
	tid_thdIncTimerSeconds = osThreadNew (thdIncTimerSeconds, NULL, NULL);
	if (!tid_thdIncTimerSeconds) return(-1);

	return(0);
}
 
void thdIncTimerSeconds (void *argument) {
 
	while (1) {
		// Wait until thdIncTimer decides we need to be called
		osSemaphoreAcquire(semIncTimerSeconds, osWaitForever);
		
		osMutexAcquire(mutTimerSecond, osWaitForever);
		timer_second++;
		timer_second%=60;
		
		
		// Increment minutes 
		if(timer_second==0){
		//	osSemaphoreRelease(semIncHours);
		}
		
		osMutexRelease(mutTimerSecond);
		
		osThreadFlagsSet(tid_thdDisplayTimer, UPDATE_SECONDS);
	}
}
