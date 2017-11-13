#include "cmsis_os2.h"                                        // CMSIS RTOS header file
#include "rtx_os.h"
#include "rtosClockObjects.h"
#include "Board_GLCD.h" 
 
/*----------------------------------------------------------------------------
*      DisplayClock:: Writes the time to the graphic LCD
*-------------------------------------------------------------------------------*/
 
#define XOFFSET 6  // time horizontal offset from left in characters
#define YOFFSET .5	// time vertical offset from top in characters
 
#define LCDWIDTH 320
#define LCDHEIGHT 240
#define CHARWIDTH 16
#define CHARHEIGHT 24
 
 
void thdDisplayClock (void *argument);                                 // thread function
osThreadId_t tid_thdDisplayClock;                                      // thread id

int Init_thdDisplayClock (void) {
 
  tid_thdDisplayClock = osThreadNew (thdDisplayClock, NULL, NULL);
	osThreadFlagsSet(tid_thdDisplayClock, 0xF);

  if (!tid_thdDisplayClock) return(-1);
  return(0);
}

// Display a numeric component of the clock
static void drawNumber(uint32_t number, uint32_t offset){
	GLCD_DrawChar( (XOFFSET+offset)*CHARWIDTH,YOFFSET*CHARHEIGHT, 0x30 + number/10);
	GLCD_DrawChar( (XOFFSET+offset+1)*CHARWIDTH,YOFFSET*CHARHEIGHT, 0x30 + number%10);
}
void thdDisplayClock (void *argument) {
	uint32_t lminute, lsecond, lhour;
	
	while(1){
		uint32_t tflags = osThreadFlagsWait(0xF, osFlagsWaitAny, osWaitForever);

		// Get values of shared resources
		osMutexAcquire(mutSecond, osWaitForever);
		lsecond = second;
		osMutexRelease(mutSecond);
		
		osMutexAcquire(mutMinute, osWaitForever);
		lminute = minute;
		osMutexRelease(mutMinute);
		
		osMutexAcquire(mutHour, osWaitForever);
		lhour = hour;
		osMutexRelease(mutHour);
		
		if(tflags & UPDATE_HOURS){
			drawNumber(lhour, 0);
		}
		if (tflags & UPDATE_MINUTES){
			drawNumber(lminute, 3);
		}
		if(tflags & UPDATE_SECONDS){
			drawNumber(lsecond, 6);
		}

		if(tflags & UPDATE_COLONS){
			GLCD_DrawChar( (XOFFSET+2)*CHARWIDTH,YOFFSET*CHARHEIGHT, ':');
			GLCD_DrawChar( (XOFFSET+5)*CHARWIDTH,YOFFSET*CHARHEIGHT, ':');
		}
	}
}
