#include "cmsis_os2.h"                                     
#include "rtx_os.h"
#include "rtosClockObjects.h"
 
/* This thread increments seconds every tick
		Upon reaching second=60, seconds reverts to 0, and minutes are incremented */
 
void thdIncSeconds (void *argument);                                 // thread function
osThreadId_t tid_thdIncSeconds;                                      // thread id
 
int Init_thdIncSeconds (void) {
 
  tid_thdIncSeconds = osThreadNew (thdIncSeconds, NULL, NULL);
  if (!tid_thdIncSeconds) return(-1);
  
  return(0);
}
 
void thdIncSeconds (void *argument) {
 
  while (1) {
		// Wait a tick
		osDelay(1000);
   
		// Increment seconds
		osMutexAcquire(mutSecond, osWaitForever);
		second++;
		second%=60;
		
		// Increment minutes
		if(second==0){
			osSemaphoreRelease(semIncMinutes);
		}
		
		osMutexRelease(mutSecond);
		osThreadFlagsSet(tid_thdDisplayClock, UPDATE_SECONDS);
		
  }
}
