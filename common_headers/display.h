#ifndef COMMON_HEADER_DISPLAY
#define COMMON_HEADER_DISPLAY
#include "Board_GLCD.h"
#define LCDWIDTH 320
#define LCDHEIGHT 240
#define CHARWIDTH 6
#define CHARHEIGHT 8
#define BACKGROUND_COLOR GLCD_COLOR_BLACK
#define FOREGROUND_COLOR GLCD_COLOR_GREEN

#include "rtx_os.h"
// Only one thread can access the GLCD at a time
extern osMutexId_t mutGLCD;

void setupDisplay(void);

#endif
