#include "cmsis_os2.h"                                     
#include "rtx_os.h"
#include "rtosClockObjects.h"
 
/* This thread increments the milliseconds of the timer every tick
 * as well as manages the timer in general*/
 
void thdIncTimer (void *argument);                                 // thread function
osThreadId_t tid_thdIncTimer;                                      // thread id
 
int Init_thdIncTimer (void) {
 
	tid_thdIncTimer = osThreadNew (thdIncTimer, NULL, NULL);
	if (!tid_thdIncTimer) return(-1);

	return(0);
}
 
static void resetTimer(){
				osMutexAcquire(mutTimerMillisecond, osWaitForever);
				timer_millisecond = 0;
				osMutexRelease(mutTimerMillisecond);
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
			osDelay(1);
	   
			// Increment seconds
			osMutexAcquire(mutTimerMillisecond, osWaitForever);
			timer_millisecond++;
			timer_millisecond%=1000;
			
			// Increment minutes
			if(timer_millisecond==0){
				//osSemaphoreRelease(semIncMinutes);
			}
			
			osMutexRelease(mutTimerMillisecond);
			osThreadFlagsSet(tid_thdDisplayTimer, UPDATE_MILLISECONDS);

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
