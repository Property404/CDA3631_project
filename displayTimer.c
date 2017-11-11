#include "cmsis_os2.h"                                        // CMSIS RTOS header file
#include "rtx_os.h"
#include "rtosClockObjects.h"
#include "Board_GLCD.h" 
#include "GLCD_Config.h"
/*----------------------------------------------------------------------------
*      DisplayTimer:: Writes the stopwatch to the LCD
*-------------------------------------------------------------------------------*/
void thdDisplayTimer (void *argument);                                 // thread function
osThreadId_t tid_thdDisplayTimer;                                      // thread id

int Init_thdDisplayTimer(void) {
 
  tid_thdDisplayTimer= osThreadNew (thdDisplayTimer, NULL, NULL);
  if (!tid_thdDisplayTimer) return(-1);
	osThreadFlagsSet(tid_thdDisplayTimer, UPDATE_MILLISECONDS | UPDATE_SECONDS | UPDATE_MINUTES | UPDATE_COLONS);
  return(0);
}

#define XOFFSET 6  // time horizontal offset from left in characters
#define YOFFSET 9	// time vertical offset from top in characters
 
#define LCDWIDTH 320
#define LCDHEIGHT 240
#define CHARWIDTH 16
#define CHARHEIGHT 24

// Display a numeric component of the stopwatch
static void drawNumber(uint32_t number, uint32_t offset){
	GLCD_DrawChar((XOFFSET+offset)*CHARWIDTH,YOFFSET*CHARHEIGHT, 0x30 + number/10);
	GLCD_DrawChar((XOFFSET+offset+1)*CHARWIDTH,YOFFSET*CHARHEIGHT, 0x30 + number%10);
}
void thdDisplayTimer (void *argument) {
	uint32_t lminute, lsecond, lmillisecond;
	
	while(1){
		// We're waiting for any component of the stopwatch to update
		uint32_t tflags = osThreadFlagsWait(UPDATE_MINUTES | UPDATE_MILLISECONDS | UPDATE_SECONDS | UPDATE_COLONS, osFlagsWaitAny, osWaitForever);

		// Get values of shared resources
		osMutexAcquire(mutTimerMillisecond, osWaitForever);
		lmillisecond = timer_millisecond;
		osMutexRelease(mutTimerMillisecond);
		
		osMutexAcquire(mutTimerSecond, osWaitForever);
		lsecond = timer_second;
		osMutexRelease(mutTimerSecond);
		
		osMutexAcquire(mutTimerMinute, osWaitForever);
		lminute = timer_minute;
		osMutexRelease(mutTimerMinute);
		
		if (tflags & UPDATE_MINUTES){
			drawNumber(lminute, 0);
		}
		if(tflags & UPDATE_SECONDS){
			drawNumber(lsecond, 3);
		}
		if(tflags & UPDATE_MILLISECONDS){
			drawNumber(lmillisecond, 6);
		}
		if(tflags & UPDATE_COLONS){
			GLCD_DrawChar((XOFFSET+2)*CHARWIDTH,YOFFSET*CHARHEIGHT, ':');
			GLCD_DrawChar((XOFFSET+5)*CHARWIDTH,YOFFSET*CHARHEIGHT, ':');
		}
	}
}
