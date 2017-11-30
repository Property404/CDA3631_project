#include "cmsis_os2.h"                                        // CMSIS RTOS header file
#include "rtx_os.h"
#include "timer.h"
#include "display_time_components.h"

/*----------------------------------------------------------------------------
*      DisplayTimer:: Writes the stopwatch to the LCD
*-------------------------------------------------------------------------------*/
void thdDisplayTimer (void *argument);                                 // thread function
osThreadId_t tid_thdDisplayTimer;                                      // thread id

int Init_thdDisplayTimer(void) {
  tid_thdDisplayTimer= osThreadNew (thdDisplayTimer, NULL, NULL);
  if (!tid_thdDisplayTimer) return(-1);
	osThreadFlagsSet(tid_thdDisplayTimer, UPDATE_ALL_TIME_COMPONENTS);
  return(0);
}

#define YOFFSET 28	// time vertical offset from top in characters

void thdDisplayTimer (void *argument) {
	uint32_t lminute, lsecond, lmillisecond;
	
	while(1){
		// We're waiting for any component of the stopwatch to update
		uint32_t tflags = osThreadFlagsWait(UPDATE_ALL_TIME_COMPONENTS, osFlagsWaitAny, osWaitForever);

		// Get values of shared resources
		osMutexAcquire(mutTimerCentisecond, osWaitForever);
		lmillisecond = timer_centisecond;
		osMutexRelease(mutTimerCentisecond);
		
		osMutexAcquire(mutTimerSecond, osWaitForever);
		lsecond = timer_second;
		osMutexRelease(mutTimerSecond);
		
		osMutexAcquire(mutTimerMinute, osWaitForever);
		lminute = timer_minute;
		osMutexRelease(mutTimerMinute);
		
		if (tflags & UPDATE_MINUTES){
			drawNumber(lminute, 0, YOFFSET);
		}
		if(tflags & UPDATE_SECONDS){
			drawNumber(lsecond, 3, YOFFSET);
		}
		if(tflags & UPDATE_CENTISECONDS){
			drawNumber(lmillisecond, 6, YOFFSET);
		}
		if(tflags & UPDATE_COLONS){
			drawColons(YOFFSET);
		}
	}
}
