#include "cmsis_os2.h"
#include "rtx_os.h"
#include "clock.h"
 
/*----------------------------------------------------------------------------
*      thdIncHours: increments hours
 *---------------------------------------------------------------------------*/
 
void thdIncHours (void *argument);                                 // thread function
osThreadId_t tid_thdIncHours;                                      // thread id
 
int Init_thdIncHours (void) {
 
  tid_thdIncHours = osThreadNew (thdIncHours, NULL, NULL);
  if (!tid_thdIncHours) return(-1);
  
  return(0);
}
 
void thdIncHours (void *argument) {
 
  while (1) {
		// Wait until thdIncMinutes decides we should increment Hours
		osSemaphoreAcquire(semIncHours, osWaitForever);
		
		// Increment Hours
		osMutexAcquire(mutHour, osWaitForever);
		hour++;
		hour%=24;
		
		// Update and go back to thdIncMinutes
		osMutexRelease(mutHour);
		
		// Update value on screen (gotta do this because the WAKEUP button can update faster than 1 HZ)
		osThreadFlagsSet(tid_thdDisplayClock, UPDATE_HOURS);

  }
}
