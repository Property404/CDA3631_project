#include "cmsis_os2.h"                                        // CMSIS RTOS header file
#include "rtx_os.h"
#include "Board_GLCD.h"
#include "display_time_components.h"
#include "clock.h"
 
/*----------------------------------------------------------------------------
*      DisplayClock:: Writes the time to the graphic LCD
*-------------------------------------------------------------------------------*/
 
#define YOFFSET .5	// time vertical offset from top in characters

 
void thdDisplayClock (void *argument);                                 // thread function
osThreadId_t tid_thdDisplayClock;                                      // thread id

int Init_thdDisplayClock (void) {
 
	tid_thdDisplayClock = osThreadNew (thdDisplayClock, NULL, NULL);
	osThreadFlagsSet(tid_thdDisplayClock, UPDATE_ALL_TIME_COMPONENTS);
	if (!tid_thdDisplayClock) return(-1);
	return(0);
}

void thdDisplayClock (void *argument) {
	while(1){
		uint32_t tflags = osThreadFlagsWait(UPDATE_ALL_TIME_COMPONENTS, osFlagsWaitAny, osWaitForever);

		// Get values of shared resources
		osMutexAcquire(mutSecond, osWaitForever);
		const uint32_t lsecond = second;
		osMutexRelease(mutSecond);
		
		osMutexAcquire(mutMinute, osWaitForever);
		const uint32_t lminute = minute;
		osMutexRelease(mutMinute);
		
		osMutexAcquire(mutHour, osWaitForever);
		const uint32_t lhour = hour;
		osMutexRelease(mutHour);
		
		if(tflags & UPDATE_HOURS){
			drawNumber(lhour, 0, YOFFSET);
		}
		if (tflags & UPDATE_MINUTES){
			drawNumber(lminute, 3, YOFFSET);
		}
		if(tflags & UPDATE_SECONDS){
			drawNumber(lsecond, 6, YOFFSET);
		}

		if(tflags & UPDATE_COLONS){
			drawColons(YOFFSET);
		}
	}
}
