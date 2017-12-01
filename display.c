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
