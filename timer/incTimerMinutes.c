#include "cmsis_os2.h"
#include "rtx_os.h"
#include "timer.h"
 
/*Increments minutes for the timer.*/
 
void thdIncTimerMinutes (void *argument);                                 // thread function
osThreadId_t tid_thdIncTimerMinutes;                                      // thread id
 
int Init_thdIncTimerMinutes (void) {
	tid_thdIncTimerMinutes = osThreadNew (thdIncTimerMinutes, NULL, NULL);
	if (!tid_thdIncTimerMinutes) return(-1);

	return(0);
}
 
void thdIncTimerMinutes (void *argument) {
 
	while (1) {
		// Wait until thdIncTimerSeconds decides we need to be called
		osSemaphoreAcquire(semIncTimerMinutes, osWaitForever);
		
		osMutexAcquire(mutTimerMinute, osWaitForever);
		timer_minute++;
		osMutexRelease(mutTimerMinute);
		
		osThreadFlagsSet(tid_thdDisplayTimer, UPDATE_MINUTES);
	}
}
