#include "cmsis_os2.h"                                     
#include "rtx_os.h"
#include "rtosClockObjects.h"
 
/* This thread increments the milliseconds of the timer every 10 ticks
 * as well as manages the timer in general*/
 
void thdIncTimer (void *argument);                                 // thread function
osThreadId_t tid_thdIncTimer;                                      // thread id
 
int Init_thdIncTimer (void) {
 
	tid_thdIncTimer = osThreadNew (thdIncTimer, NULL, NULL);
	if (!tid_thdIncTimer) return(-1);

	return(0);
}
 
static void resetTimer(){
	osMutexAcquire(mutTimerCentisecond, osWaitForever);
	timer_centisecond = 0;
	osMutexRelease(mutTimerCentisecond);
	osMutexAcquire(mutTimerSecond, osWaitForever);
	timer_second = 0;
	osMutexRelease(mutTimerSecond);
	osMutexAcquire(mutTimerMinute, osWaitForever);
	timer_minute = 0;
	osMutexRelease(mutTimerMinute);
}
void thdIncTimer (void *argument) {
	while(1) {
		// Wait until it's time to start the timer
		osSemaphoreAcquire(semTimer, osWaitForever);
		// Cover synchronization errors
		resetTimer();
		// Remove "extra" presses
		while(osSemaphoreAcquire(semTimer, 0) == osOK);

		while (1) {
			// Wait a tick
			osDelay(10);
	   
			// Increment seconds
			osMutexAcquire(mutTimerCentisecond, osWaitForever);
			timer_centisecond++;
			timer_centisecond%=100;
			
			// Increment minutes
			if(timer_centisecond==0){
				osSemaphoreRelease(semIncTimerSeconds);
			}
			
			osMutexRelease(mutTimerCentisecond);
			osThreadFlagsSet(tid_thdDisplayTimer, UPDATE_CENTISECONDS);

			if(osSemaphoreAcquire(semTimer, 0) == osOK){
				// Time to pause!
				// Remove "extra" presses
				while(osSemaphoreAcquire(semTimer, 0) == osOK);
				// Just wait until it's time to reset
				osSemaphoreAcquire(semTimer, osWaitForever);
				// Reset and go back to waiting
				resetTimer();
				osThreadFlagsSet(tid_thdDisplayTimer, UPDATE_ALL_TIME_COMPONENTS);
				break;
			}
			
		}
	}
}
