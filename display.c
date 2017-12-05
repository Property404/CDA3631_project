#include "display.h"
#include "GLCD_Config.h"
#include "rtos_util.h"

// Modified version of the GLCD_Font_6x8
extern GLCD_FONT tinyFont;
osMutexId_t mutGLCD;
void setupDisplay(){
	// Basic GLCD set up
	GLCD_Initialize();
	GLCD_SetBackgroundColor(BACKGROUND_COLOR);
	GLCD_SetForegroundColor(FOREGROUND_COLOR);
	GLCD_ClearScreen(); 
	GLCD_SetFont(&tinyFont);
	
	// We only want one thread to acccess the GLCD at a time
	mutGLCD = newMutexOrDie();
}

#include "time_components.h"
#define XOFFSET 23  // time horizontal offset from left in characters


// Make these functions `static inline`, cause it's kinda silly to
// have a separate C file

// Display a two-digit numeric component of the clock or timer
void drawNumber(const uint32_t number, const uint32_t char_offset, const float y_offset){
	osMutexAcquire(mutGLCD, osWaitForever);
	GLCD_DrawChar((XOFFSET+char_offset)*CHARWIDTH,y_offset*CHARHEIGHT, 0x30 + number/10);
	GLCD_DrawChar((XOFFSET+char_offset+1)*CHARWIDTH,y_offset*CHARHEIGHT, 0x30 + number%10);
	osMutexRelease(mutGLCD);
}
// Display the colons between the numeric components
void drawColons(const float y_offset){
	osMutexAcquire(mutGLCD, osWaitForever);
	GLCD_DrawChar((XOFFSET+2)*CHARWIDTH,y_offset*CHARHEIGHT, ':');
	GLCD_DrawChar((XOFFSET+5)*CHARWIDTH,y_offset*CHARHEIGHT, ':');
	osMutexRelease(mutGLCD);
}

#undef XOFFSET
