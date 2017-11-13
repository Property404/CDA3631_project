#include "cmsis_os2.h"
#include "rtx_os.h"
#include "clock.h"
 
/*Increments minutes. When minutes reaches 60, minute is reset and hour is incremented */
 
void thdIncMinutes (void *argument);                                 // thread function
osThreadId_t tid_thdIncMinutes;                                      // thread id
 
int Init_thdIncMinutes (void) {
 
  tid_thdIncMinutes = osThreadNew (thdIncMinutes, NULL, NULL);
  if (!tid_thdIncMinutes) return(-1);
  
  return(0);
}
 
void thdIncMinutes (void *argument) {
 
  while (1) {
		// Wait until thdIncSeconds decides we should increment minutes
		osSemaphoreAcquire(semIncMinutes, osWaitForever);
		
		// Increment minutes
		osMutexAcquire(mutMinute, osWaitForever);
		minute++;
		minute%=60;
		
		
		// Increment Hours
		if(minute==0){
			osSemaphoreRelease(semIncHours);

		}
		
		osMutexRelease(mutMinute);
		
		// Update value on screen (gotta do this because the USER button can update faster than 1 HZ)
		osThreadFlagsSet(tid_thdDisplayClock, UPDATE_MINUTES);
		
		
  }
}
